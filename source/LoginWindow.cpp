/*
 * =====================================================================================
 *
 *       Filename:  LoginWindow.cpp
 *
 *    Description:
 *
 *        Created:  02/12/2017 02:39:12
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <QApplication>
#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "IntraSession.hpp"
#include "LoginWindow.hpp"

LoginWindow::LoginWindow(const Keyring &keyring, QWidget *parent) : QDialog(parent), m_keyring(keyring) {
	setWindowTitle("Login to Epitech Intranet");
	resize(350, minimumSize().height());

	setupWidgets();

	// m_keyring.remove("eifl_autologin");
	// m_keyring.remove("eifl_login");
	// m_keyring.remove("eifl_password");

	if (m_keyring.has("eifl_autologin")) {
		m_autologinWidget.setText(m_keyring.get("eifl_autologin"));
		// m_rememberMeWidget.setCheckState(Qt::Checked);
	}

	// if (m_keyring.has("eifl_login"))
	// 	m_loginWidget.setText(m_keyring.get("eifl_login"));
    //
	// if (m_keyring.has("eifl_password")) {
	// 	m_passwordWidget.setText(m_keyring.get("eifl_password"));
	// 	m_rememberMeWidget.setCheckState(Qt::Checked);
	// }
}

void LoginWindow::login() {
	// if (m_loginWidget.text() != m_keyring.get("eifl_login"))
	// 	emit databaseReloadRequested();

	m_keyring.store("eifl_autologin", m_autologinWidget.text());
	// m_keyring.store("eifl_login", m_loginWidget.text());
	// m_keyring.store("eifl_password", m_passwordWidget.text());

	int loginStatus = IntraSession::getInstance().login(m_keyring);
	if (loginStatus == 200) {
		// if (m_rememberMeWidget.checkState() == Qt::Unchecked) {
			// m_keyring.remove("eifl_password");
			// m_keyring.remove("eifl_autologin");
		// }

		accept();

		if (m_autologinWidget.text() != m_keyring.get("eifl_autologin"))
			emit databaseReloadRequested();
	}
	else if (loginStatus == 401) {
		// m_errorLabel.setText("Error: Bad login/password.");
		m_errorLabel.setText("Error: Bad autologin link.");
		m_errorLabel.show();
		// m_passwordWidget.clear();
		m_autologinWidget.clear();
	}
	else if (loginStatus == 0) {
		m_errorLabel.setText("Error: Request has timeout. Intra may be down.");
		m_errorLabel.show();
	}
	else {
		m_errorLabel.setText("Error: Failed to login. Code: " + QString::number(loginStatus));
		m_errorLabel.show();
	}
}

void LoginWindow::setupWidgets() {
	m_errorLabel.setStyleSheet("QLabel { color: red; }");
	m_errorLabel.hide();

	m_autologinHelp.setText("<a href='https://intra.epitech.eu/admin/autolog'>Click here to get your autologin link.</a>");
	m_autologinHelp.setOpenExternalLinks(true);

	// m_passwordWidget.setEchoMode(QLineEdit::Password);

	auto *formLayoutWidget = new QWidget;
	auto *formLayout = new QFormLayout(formLayoutWidget);
	formLayout->addRow("Autologin link:", &m_autologinWidget);
	// formLayout->addRow("Login:", &m_loginWidget);
	// formLayout->addRow("Password:", &m_passwordWidget);

	auto *loginButton = new QPushButton("Login");
	connect(loginButton, &QPushButton::clicked, this, &LoginWindow::login);

	auto* line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	auto* line1 = new QFrame();
	line1->setFrameShape(QFrame::HLine);
	line1->setFrameShadow(QFrame::Sunken);

	auto* line2 = new QFrame();
	line2->setFrameShape(QFrame::HLine);
	line2->setFrameShadow(QFrame::Sunken);

	auto *offlineButton = new QPushButton("Offline mode");
	connect(offlineButton, &QPushButton::clicked, this, &QDialog::reject);

	auto *quitButton = new QPushButton("Quit");
	connect(quitButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(quitButton, &QPushButton::clicked, this, &LoginWindow::quitButtonPressed);

	auto *verticalLayout = new QVBoxLayout(this);
	verticalLayout->addWidget(&m_errorLabel);
	verticalLayout->addWidget(formLayoutWidget);
	// verticalLayout->addWidget(&m_rememberMeWidget);
	verticalLayout->addWidget(line);
	verticalLayout->addWidget(&m_autologinHelp);
	verticalLayout->addWidget(line1);
	verticalLayout->addWidget(loginButton);
	verticalLayout->addWidget(line2);
	verticalLayout->addWidget(offlineButton);
	verticalLayout->addWidget(quitButton);
}

