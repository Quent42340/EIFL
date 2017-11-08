/*
 * =====================================================================================
 *
 *       Filename:  EventListWidget.cpp
 *
 *    Description:
 *
 *        Created:  16/10/2017 01:28:41
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>

#include "IntraData.hpp"
#include "EventListWidget.hpp"
#include "IntraData.hpp"

EventListWidget::EventListWidget(QWidget *parent) : QDockWidget(tr("Events"), parent) {
	m_eventListWidget.setColumnCount(7);
	m_eventListWidget.setHeaderLabels({"", tr("Start"), tr("End"), tr("Room"), tr("Type"), tr("Module"), tr("Name")});
	m_eventListWidget.setRootIsDecorated(false);
	m_eventListWidget.setSortingEnabled(true);
	m_eventListWidget.header()->setSectionResizeMode(QHeaderView::Fixed);
	m_eventListWidget.sortItems(1, Qt::AscendingOrder);
	m_eventListWidget.setColumnWidth(0, 27);
	m_eventListWidget.setColumnWidth(5, 2 * m_eventListWidget.columnWidth(5));

	setWidget(&m_eventListWidget);
}

void EventListWidget::update() {
	m_eventListWidget.clear();

	unsigned int eventCount = 0;
	auto eventList = IntraData::getInstance().eventList();
	for (auto it : eventList) {
		if (m_semesters.contains(it.second.activity().module().semester())
		 && (it.second.beginDate().date() == m_date || it.second.endDate().date() == m_date)) {
			if ((!m_isCurrentSemesterEnabled || (it.second.activity().module().semester() == IntraData::getInstance().userInfo().currentSemester() || it.second.activity().module().semester() == 0))
		     && (!m_isRegisteredModulesEnabled || it.second.activity().module().isRegistered())
		     && (!m_isRegisteredEventsEnabled || it.second.isRegistered())) {
				auto *item = new QTreeWidgetItem(&m_eventListWidget);
				item->setText(1, it.second.beginDate().toString("HH:mm") + it.second.activity().appointmentDate().toString(" (HH:mm)"));
				item->setText(2, it.second.endDate().toString("HH:mm"));
				item->setText(3, it.second.roomName());
				item->setText(4, it.second.activity().typeTitle());
				item->setText(5, it.second.activity().module().name());
				item->setText(6, it.second.activity().name());

				if (it.second.isMissed()) {
					item->setIcon(0, QIcon(":/missed.svg"));
					item->setText(0, " 0");

					for (int i = 0 ; i < item->columnCount() ; ++i)
						item->setBackgroundColor(i, Qt::darkRed);
				}
				else if (it.second.isRegistered()) {
					item->setIcon(0, QIcon(":/registered.svg"));
					item->setText(0, " 1");
				}
				else if (it.second.isRegistrable() && it.second.isValid()) {
					item->setIcon(0, QIcon(":/registrable.svg"));
					item->setText(0, " 2");
				}
				else {
					item->setIcon(0, QIcon(":/locked.svg"));
					item->setText(0, " 3");
				}
			}

			++eventCount;
		}
	}

	setWindowTitle(tr("Events") + " (" + QString::number(m_eventListWidget.topLevelItemCount()) + "/" + QString::number(eventCount) + ")");
}

void EventListWidget::setFilters(bool isCurrentSemesterEnabled, bool isRegisteredModulesEnabled, bool isRegisteredEventsEnabled) {
	m_isCurrentSemesterEnabled = isCurrentSemesterEnabled;
	m_isRegisteredModulesEnabled = isRegisteredModulesEnabled;
	m_isRegisteredEventsEnabled = isRegisteredEventsEnabled;

	update();
}

