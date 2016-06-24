#include "teamform.h"
#include "ui_teamform.h"
#include <QSettings>
#include "mainapplication.h"

TeamForm::TeamForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeamForm)
{
    ui->setupUi(this);
    QSettings settings;
    QStringList known = settings.value("team_domains").toStringList();
    known.sort(Qt::CaseInsensitive);
    ui->listWidget->addItems(known);
    if(known.isEmpty()){ 
      ui->radio_newteam->setChecked(true);
      ui->radio_knownteam->setEnabled(false);
      ui->listWidget->setEnabled(false);
      ui->lineEdit->setFocus();
    }else{ ui->radio_knownteam->setChecked(true); }
    ui->pushButton->setEnabled(false);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onTextChanged()) );
    connect(ui->radio_newteam, SIGNAL(toggled(bool)), this, SLOT(onTextChanged()) );
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onClick()));
    setWindowIcon(QIcon(QString("://images/png/icon32.png")));
    onTextChanged();
}

TeamForm::~TeamForm()
{
    delete ui;
}

void TeamForm::onTextChanged()
{
    QString text = getTeamSelection();
    if (text.trimmed() == "")
    {
        ui->pushButton->setEnabled(false);
    }
    else
    {
        ui->pushButton->setEnabled(true);
    }
  ui->lineEdit->setEnabled( ui->radio_newteam->isChecked() );
  ui->listWidget->setEnabled(ui->radio_knownteam->isChecked() );
}

void TeamForm::onClick()
{
    QString team = getTeamSelection();
    QSettings settings;
    QStringList known = settings.value("team_domains").toStringList();
    known << team;
    known.removeDuplicates();
    settings.setValue("team_domains", known);
    emit openTeam(team); //seng the signal for the main window to open this team
    //((MainApplication *) mApp->instance())->getMainWindow()->show();
    //close();
}

QString TeamForm::getTeamSelection(){
  if(ui->radio_newteam->isChecked()){ return ui->lineEdit->text(); }
  else{ 
    QListWidgetItem *it = ui->listWidget->currentItem();
    if(it!=0){ return it->text(); }
    else{ return ""; }
  }
}
