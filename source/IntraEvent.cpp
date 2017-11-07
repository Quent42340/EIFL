/*
 * =====================================================================================
 *
 *       Filename:  IntraEvent.cpp
 *
 *    Description:
 *
 *        Created:  16/10/2017 02:01:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <QJsonArray>
#include <QSqlRecord>
#include <QVariant>

#include "IntraEvent.hpp"
#include "IntraSession.hpp"

IntraEvent::IntraEvent(const IntraActivity &activity, const QJsonObject &jsonObject) : m_activity(activity) {
	m_id = jsonObject.value("code").toString().mid(6).toUInt();

	// FIXME: Find a better way to get this information
	m_isRegistrable = m_activity.isRegistrable();
	m_isRegistered = !jsonObject.value("already_register").isNull();
	m_isMissed = jsonObject.value("user_status").toString() == "absent";
	m_isTokenWritable = jsonObject.value("allow_token").toString().toInt();

	QStringList roomInfos = jsonObject.value("location").toString().split('/');
	if (roomInfos.size() > 2)
		m_buildingName = roomInfos.at(2);
	if (roomInfos.size() > 3)
		m_roomName = roomInfos.at(3);

	m_beginDate = QDateTime::fromString(jsonObject.value("begin").toString(), "yyyy-MM-dd HH:mm:ss");
	m_endDate = QDateTime::fromString(jsonObject.value("end").toString(), "yyyy-MM-dd HH:mm:ss");

	if (activity.isAppointment() && activity.module().isRegistered()) {
		QJsonDocument json = IntraSession::getInstance().get(m_activity.module().link() + "/acti-" + QString::number(m_activity.id()) + "/rdv");

		// FIXME: Find a better way to get this information
		m_isRegistered = json.object().value("student_registered").toBool()
		              && json.object().value("with_project").toBool();

		// FIXME: Sometimes userId may be used instead of teamId, how to get current user id?
		if (m_isRegistered) {
			unsigned int teamId = json.object().value("group").toObject().value("id").toInt();
			QJsonArray eventArray = json.object().value("slots").toArray();
			for (QJsonValue eventValue : eventArray) {
				if (eventValue.toObject().value("codeevent").toString() == "event-" + QString::number(m_id)) {
					QJsonArray slotArray = eventValue.toObject().value("slots").toArray();
					for (QJsonValue slotValue : slotArray) {
						if (slotValue.toObject().value("id_team").toString().toInt() == teamId) {
							m_isRegistered = true;
							m_appointmentDate = QDateTime::fromString(slotValue.toObject().value("date").toString(), "yyyy-MM-dd HH:mm:ss");
						}
					}
				}
			}
		}
	}
}

IntraEvent::IntraEvent(const IntraActivity &activity, const QSqlQuery &sqlQuery) : m_activity(activity) {
	m_id = sqlQuery.value(sqlQuery.record().indexOf("id")).toUInt();

	m_buildingName = sqlQuery.value(sqlQuery.record().indexOf("building_name")).toString();
	m_roomName = sqlQuery.value(sqlQuery.record().indexOf("room_name")).toString();

	m_beginDate = sqlQuery.value(sqlQuery.record().indexOf("begin_date")).toDateTime();
	m_endDate = sqlQuery.value(sqlQuery.record().indexOf("end_date")).toDateTime();
	m_appointmentDate = sqlQuery.value(sqlQuery.record().indexOf("appointment_date")).toDateTime();

	m_isRegistrable = sqlQuery.value(sqlQuery.record().indexOf("is_registrable")).toBool();
	m_isRegistered = sqlQuery.value(sqlQuery.record().indexOf("is_registered")).toBool();
	m_isMissed = sqlQuery.value(sqlQuery.record().indexOf("is_missed")).toBool();
}

// IntraEvent::IntraEvent(const QJsonObject &jsonObject) {
// 	m_name = jsonObject.value("acti_title").toString();
//
// 	m_typeCode = jsonObject.value("type_code").toString();
// 	m_typeTitle = jsonObject.value("type_title").toString();
//
// 	m_semester = jsonObject.value("semester").toInt();
//
// 	m_moduleName = jsonObject.value("titlemodule").toString();
// 	m_isModuleRegistered = jsonObject.value("module_registered").toBool();
//
// 	m_isAppointment = jsonObject.value("is_rdv").toBool();
// 	m_appointmentDate = QDateTime::fromString(jsonObject.value("rdv_group_registered").toString().left(19), "yyyy-MM-dd HH:mm:ss");
//
// 	m_isRegistrable = jsonObject.value("allow_register").toBool();
// 	m_isRegistered = jsonObject.value("event_registered").toBool() ||
// 	                 jsonObject.value("event_registered").toString() == "registered" ||
// 	                 !m_appointmentDate.isNull();
// 	m_isMissed = jsonObject.value("event_registered").toString() == "absent";
// 	m_isTokenWritable = jsonObject.value("allow_token").toBool();
//
// 	QStringList roomInfos = jsonObject.value("room").toObject().value("code").toString().split('/');
// 	if (roomInfos.size() > 2)
// 		m_buildingName = roomInfos.at(2);
// 	if (roomInfos.size() > 3)
// 		m_roomName = roomInfos.at(3);
//
// 	m_beginDate = QDateTime::fromString(jsonObject.value("start").toString(), "yyyy-MM-dd HH:mm:ss");
// 	m_endDate = QDateTime::fromString(jsonObject.value("end").toString(), "yyyy-MM-dd HH:mm:ss");
// }

// void IntraEvent::update() {
// 	QJsonDocument json = IntraSession::getInstance().get(m_activity.module().link() + "acti-" + QString::number(m_activity.id()) + "/" + m_id + "/");
//
// 	// FIXME: This may not work now
// 	m_isAppointment = json.object().value("is_rdv").toBool();
// 	m_appointmentDate = QDateTime::fromString(json.object().value("rdv_group_registered").toString().left(19), "yyyy-MM-dd HH:mm:ss");
// }

