/*
 * =====================================================================================
 *
 *       Filename:  IntraDatabase.hpp
 *
 *    Description:
 *
 *        Created:  06/11/2017 01:38:10
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INTRADATABASE_HPP_
#define INTRADATABASE_HPP_

#include <functional>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>

#include "IntraActivity.hpp"
#include "IntraModule.hpp"

#include <iostream>

class IntraDatabaseThreadPool;

class IntraDatabase : public QObject {
	Q_OBJECT

	public:
		void open(const QString &path);
		void clear() const;
		void update();
		void updateUser() const;
		void updateNotifications() const;
		void updateUnits() const;
		void updateActivities(IntraModule unit) const;
		void updateEvents(const IntraActivity &activity, const QJsonObject &jsonObject) const;
		void updateProjects(const IntraActivity &activity) const;

		void setThreadPool(IntraDatabaseThreadPool *threadPool) { m_threadPool = threadPool; }

		static void addTable(const QString &name, const std::map<QString, QVariant> &fields);
		static void removeTable(const QString &name);

	signals:
		void updateStarted() const;
		void updateProgressed(int value) const;
		void updateFinished() const;

		void userUpdateFinished() const;
		void notificationUpdateFinished() const;

		void unitUpdateProgressed(int value) const;
		void unitUpdateFinished() const;

	private:
		QSqlDatabase m_database;

		IntraDatabaseThreadPool *m_threadPool;
};

#endif // INTRADATABASE_HPP_
