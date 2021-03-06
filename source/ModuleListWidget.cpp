/*
 * =====================================================================================
 *
 *       Filename:  ModuleListWidget.cpp
 *
 *    Description:
 *
 *        Created:  17/10/2017 18:42:08
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <QHeaderView>

#include "IntraData.hpp"
#include "ModuleListWidget.hpp"

ModuleListWidget::ModuleListWidget(QWidget *parent) : QDockWidget(tr("Modules"), parent) {
	m_moduleListWidget.setHeaderLabels({"", tr("Name")});
	m_moduleListWidget.setRootIsDecorated(false);
	m_moduleListWidget.setSortingEnabled(true);
	m_moduleListWidget.header()->setSectionResizeMode(QHeaderView::Fixed);
	m_moduleListWidget.setColumnWidth(0, 27);
	m_moduleListWidget.sortItems(0, Qt::DescendingOrder);

	setWidget(&m_moduleListWidget);
}

void ModuleListWidget::update() {
	m_moduleListWidget.clear();

	auto &moduleList = IntraData::getInstance().moduleList();
	for (auto &it : moduleList) {
		if ((!m_isCurrentSemesterEnabled || (it.second.semester() == IntraData::getInstance().userInfo().currentSemester() || it.second.semester() == 0))
		 && (!m_isRegisteredModulesEnabled || it.second.isRegistered())
		 && m_semesters.contains(it.second.semester())) {
			auto *item = new QTreeWidgetItem(&m_moduleListWidget);
			item->setText(1, it.second.name());

			if (it.second.isRegistered()) {
				item->setIcon(0, QIcon(":/registered.svg"));
				item->setText(0, " 0");
			}
			else if (it.second.isRegistrable()) {
				item->setIcon(0, QIcon(":/registrable.svg"));
				item->setText(0, " 2");
			}
			else {
				item->setIcon(0, QIcon(":/locked.svg"));
				item->setText(0, " 1");
			}
		}
	}

	setWindowTitle(tr("Modules") + " (" + QString::number(m_moduleListWidget.topLevelItemCount()) + "/" + QString::number(moduleList.size()) + ")");
}

void ModuleListWidget::setFilters(bool isCurrentSemesterEnabled, bool isRegisteredModulesEnabled) {
	m_isCurrentSemesterEnabled = isCurrentSemesterEnabled;
	m_isRegisteredModulesEnabled = isRegisteredModulesEnabled;

	update();
}

