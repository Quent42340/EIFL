/*
 * =====================================================================================
 *
 *       Filename:  MainWindow.hpp
 *
 *    Description:
 *
 *        Created:  15/10/2017 18:20:45
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QMainWindow>

#include "CalendarWidget.hpp"
#include "CalendarSettingsWidget.hpp"
#include "EventInfoWidget.hpp"
#include "EventListWidget.hpp"
#include "IntraData.hpp"
#include "IntraSession.hpp"
#include "ModuleInfoWidget.hpp"
#include "ModuleListWidget.hpp"
#include "NotificationListWidget.hpp"
#include "ProjectInfoWidget.hpp"
#include "ProjectListWidget.hpp"
#include "SideBar.hpp"
#include "UserInfoWidget.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow();

		void login();
		void init(bool updateDatabase = false);

		void setupWidgets();
		void setupDocks();
		void setupTabs();
		void setupMenuBar();
		void setupStatusBar();
		void setupTrayIcon();

		void connectObjects();

		void updateWidgets();

		void showStatusTip(const QString &statusString);

		void closeEvent(QCloseEvent *event) override;
		void keyPressEvent(QKeyEvent *event) override;

		static const MainWindow &getInstance() {
			return *s_instance;
		}

		static void setInstance(const MainWindow &instance) {
			s_instance = &instance;
		}

		static const unsigned int width = 1280;
		static const unsigned int height = 720;

	private:
		void openAboutWindow();

		static const MainWindow *s_instance;

		IntraData m_intraData;
		IntraSession m_intraSession;
		Keyring m_keyring;

		SideBar m_sideBar;

		ProjectInfoWidget m_projectInfoWidget;
		ProjectListWidget m_projectListWidget;

		ModuleInfoWidget m_moduleInfoWidget;
		ModuleListWidget m_moduleListWidget;

		EventInfoWidget m_eventInfoWidget;
		EventListWidget m_eventListWidget;

		CalendarWidget m_calendarWidget;
		CalendarSettingsWidget m_calendarSettingsWidget;

		NotificationListWidget m_notificationListWidget;
		UserInfoWidget m_userInfoWidget;
};

#endif // MAINWINDOW_HPP_
