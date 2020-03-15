#include "widgets/aoroomcontrols.h"
#include "aouiloader.h"

#include <QVBoxLayout>

AORoomControls::AORoomControls(QWidget *parent, AOApplication *p_ao_app)
  : QWidget(parent), ao_app(p_ao_app)
{
  AOUiLoader loader(this, ao_app);
  QFile uiFile(":/resource/ui/room_controls.ui");
  uiFile.open(QFile::ReadOnly);
  QWidget *windowWidget = loader.load(&uiFile, this);
  QMetaObject::connectSlotsByName(this);

  QVBoxLayout *parentLayout = new QVBoxLayout;
  parentLayout->setMargin(0);
  parentLayout->addWidget(windowWidget);
  setLayout(parentLayout);

  ui_witness_testimony = findChild<QPushButton *>("witness_testimony");
  ui_cross_examination = findChild<QPushButton *>("cross_examination");
  ui_not_guilty = findChild<QPushButton *>("not_guilty");
  ui_guilty = findChild<QPushButton *>("guilty");

  ui_def_health = findChild<QProgressBar *>("def_health");
  ui_def_plus = findChild<QPushButton *>("def_plus");
  ui_def_minus = findChild<QPushButton *>("def_minus");

  ui_pro_health = findChild<QProgressBar *>("pro_health");
  ui_pro_plus = findChild<QPushButton *>("pro_plus");
  ui_pro_minus = findChild<QPushButton *>("pro_minus");
}

void AORoomControls::setHealth(HEALTH_TYPE type, int value)
{
  if (type == DEF)
    ui_def_health->setValue(value);
  else if (type == PRO)
    ui_pro_health->setValue(value);
  else
    qWarning() << "invalid health type" << type;
}

void AORoomControls::toggleJudgeControls(bool visible)
{
  QWidget * const judgeControls[] = {ui_witness_testimony, ui_cross_examination,
                                     ui_def_plus, ui_def_minus, ui_pro_plus, ui_pro_minus};
  for (auto widget : judgeControls)
  {
    widget->setVisible(visible);
  }
}

void AORoomControls::on_witnessTestimony_clicked()
{
  emit wtce(WITNESS_TESTIMONY);
}

void AORoomControls::on_crossExamination_clicked()
{
  emit wtce(CROSS_EXAMINATION);
}

void AORoomControls::on_notGuilty_clicked()
{
  emit wtce(NOT_GUILTY);
}

void AORoomControls::on_guilty_clicked()
{
  emit wtce(GUILTY);
}

void AORoomControls::on_defPlus_clicked()
{
  int newValue = ui_def_health->value() + 1;
  emit requestHealthChange(DEF, newValue);

  // Go ahead and echo back the changes, and let the server change it back
  // if we did it wrong.
  // TODO: the server should have the opposite behavior: send the HP packet
  // only if the value could not be changed to the target value.
  // Otherwise, clicking the button rapidly will have some awkward effects
  // due to lag.
  ui_def_health->setValue(newValue);
}

void AORoomControls::on_defMinus_clicked()
{
  int newValue = ui_def_health->value() - 1;
  emit requestHealthChange(DEF, newValue);
  ui_def_health->setValue(newValue);
}

void AORoomControls::on_proPlus_clicked()
{
  int newValue = ui_pro_health->value() + 1;
  emit requestHealthChange(PRO, newValue);
  ui_def_health->setValue(newValue);
}

void AORoomControls::on_proMinus_clicked()
{
  int newValue = ui_pro_health->value() - 1;
  emit requestHealthChange(PRO, newValue);
  ui_def_health->setValue(newValue);
}