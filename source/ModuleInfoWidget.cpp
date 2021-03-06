/*
 * =====================================================================================
 *
 *       Filename:  ModuleInfoWidget.cpp
 *
 *    Description:
 *
 *        Created:  17/10/2017 20:17:33
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <QFormLayout>

#include "IntraData.hpp"
#include "IntraSession.hpp"
#include "ModuleInfoWidget.hpp"

ModuleInfoWidget::ModuleInfoWidget(QWidget *parent) : QWidget(parent) {
	QFormLayout *layout = new QFormLayout(this);

	layout->addRow("Name:", &m_name);
	layout->addRow("Credits:", &m_creditCount);
}

void ModuleInfoWidget::update(QTreeWidgetItem *item) {
	if (item) {
		for (auto &it : IntraData::getInstance().moduleList()) {
			if (item->text(item->columnCount() - 1) == it.second.name()) {
				m_name.setText(it.second.name());

				m_creditCount.setNum((int)it.second.creditCount());
			}
		}
	}
}

