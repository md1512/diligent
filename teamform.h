#ifndef TEAMFORM_H
#define TEAMFORM_H

#include <QWidget>

namespace Ui
{
    class TeamForm;
}

class TeamForm : public QWidget
{
        Q_OBJECT

    public:
        explicit TeamForm(QWidget *parent = 0);
        ~TeamForm();

    private slots:
        void onTextChanged();
        void onClick();

    private:
        Ui::TeamForm *ui;

	QString getTeamSelection();

    signals:
	void openTeam(QString); //team domain
};

#endif // TEAMFORM_H
