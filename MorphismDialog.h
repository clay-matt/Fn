// Header file for class MorphismDialog
// inherits QDialog

// author Matt Clay
// version 110404

/////////////////////////////////////////////////////////

#ifndef MORPHISMDIALOG_H
#define MORPHISMDIALOG_H

#include <QDialog>

#include "Basis.h"
#include "Fn.h"

/////////////////////////////////////////////////////////

class Basis;
class FnMap;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRegExpValidator;

class MorphismDialog : public QDialog {

  Q_OBJECT

public:
    MorphismDialog(QWidget *parent = 0, const int rank = Fn_DefaultRank);

private slots:
  void changeImageRegExpValidator(int itemIndex);
  void enableIsAutoLoadButtons();
  void morphismLoad();
  void resizeImageGroupBox(int itemIndex);
  void toggleEndomorphism(int state);

private:
  Basis basis;
  Basis imageBasis;
  QLabel *rankLabel;
  QComboBox *rankComboBox;
  QCheckBox *endoCheckBox;
  QCheckBox *autoCheckBox;
  QLabel *imageRankLabel;
  QComboBox *imageRankComboBox;
  QLabel *mapLabel;
  QLineEdit *mapLineEdit;
  QLabel *imageLabel[Fn_MaxRank];
  QLineEdit *imageLineEdit[Fn_MaxRank];
  QGroupBox *imageGroupBox;
  QLabel *noAutoLabel;
  QLabel *inverseLabel[Fn_MaxRank];
  QLineEdit *inverseLineEdit[Fn_MaxRank];
  QGroupBox *inverseGroupBox;
  QPushButton *cancelButton;
  QPushButton *loadButton;
  QPushButton *dismissButton;
  QRegExpValidator *rankRegExp;

signals:
  void updateMorphismList(const QString &name, const FnMap &phi);

};

/////////////////////////////////////////////////////////

#endif // MORPHISMDIALOG_H
