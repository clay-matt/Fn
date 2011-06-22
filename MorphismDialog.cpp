//

#include <QtGui>

#include "FnMap.h"
#include "FnWord.h"
#include "MorphismDialog.h"

/////////////////////////////////////////////////////////

MorphismDialog::MorphismDialog(QWidget *parent, const int rank)
  : QDialog(parent) {

  // constructor

  int i;

  // get basis
  basis.changeRank(rank);
  imageBasis.changeRank(rank);

  // build rank and image rank combo boxes
  rankLabel = new QLabel(tr("&Rank:"));
  rankComboBox = new QComboBox;
  rankLabel->setBuddy(rankComboBox);
 
  imageRankLabel = new QLabel(tr("&Image Rank:"));
  imageRankComboBox = new QComboBox;
  imageRankLabel->setBuddy(imageRankComboBox);
 
  // build check boxes for endo and auto
  endoCheckBox = new QCheckBox(tr("&Endomorphism"));
  endoCheckBox->setChecked(true); // default = is endomorphism

  autoCheckBox = new QCheckBox(tr("&Automorphism?"));
  autoCheckBox->setEnabled(false);
  autoCheckBox->setChecked(false);
 
  // enumerate possible ranks in combo box, rank and item index are
  // offset by MIN_RANK
  QString i_QString;

  for (i = Fn_MinRank; i <= Fn_MaxRank; i++) {

    i_QString = QString::number(i);
    i_QString.rightJustified(2);

    rankComboBox->addItem(i_QString);
    imageRankComboBox->addItem(i_QString);

  }

  rankComboBox->setCurrentIndex(rank - Fn_MinRank);
  imageRankComboBox->setCurrentIndex(rank - Fn_MinRank);
  imageRankLabel->hide();
  imageRankComboBox->hide();

  // build line edits for image inputs
  mapLabel = new QLabel(tr("Morphism &Name:"));
  mapLineEdit = new QLineEdit;
  mapLabel->setBuddy(mapLineEdit);
  
  imageGroupBox = new QGroupBox(tr("&Images:"));

  rankRegExp = new QRegExpValidator(imageBasis.basisValidate(),this);

  // build line edits for inputing images
  QString element_arrow;

  for (i = 0; i < Fn_MaxRank; i++) {

    element_arrow = QString(BASIS.at(2*i)) + " ->";

    imageLabel[i] = new QLabel(element_arrow);
    imageLabel[i]->setAlignment(Qt::AlignRight);
    imageLineEdit[i] = new QLineEdit(this);
    imageLineEdit[i]->setValidator(rankRegExp);
    imageLineEdit[i]->setProperty("image", true);
  
    // hide hide line edits for larger ranks
    if (i >= rank)  {
      imageLabel[i]->hide();
      imageLineEdit[i]->hide();
    }

  }

  // build box for displaying inverse
  inverseGroupBox = new QGroupBox(tr("&Inverse:"));

  for (i = 0; i < Fn_MaxRank; i++) {

    element_arrow = QString(BASIS.at(2*i)) + " ->";

    inverseLabel[i] = new QLabel(element_arrow);
    inverseLabel[i]->setAlignment(Qt::AlignRight);
    inverseLabel[i]->hide();
    inverseLineEdit[i] = new QLineEdit(this);
    inverseLineEdit[i]->setEnabled(false);
    inverseLineEdit[i]->setProperty("image", true);
    inverseLineEdit[i]->hide();
 
  } 

  noAutoLabel = new QLabel;
  noAutoLabel->setText(tr("This morphism is not an automorphism"));
  noAutoLabel->hide();

  // buttons
  cancelButton = new QPushButton(tr("Cancel"));

  loadButton = new QPushButton(tr("&Load"));
  loadButton->setDefault(true);
  loadButton->setEnabled(false);

  dismissButton = new QPushButton(tr("&Dismiss"));
  dismissButton->hide();

  // make layouts
  QHBoxLayout *rankLayout = new QHBoxLayout;
  rankLayout->addWidget(rankLabel);
  rankLayout->addWidget(rankComboBox);
  rankLayout->addWidget(imageRankLabel);
  rankLayout->addWidget(imageRankComboBox);
  rankLayout->addStretch();

  QVBoxLayout *endoLayout = new QVBoxLayout;
  endoLayout->addWidget(endoCheckBox);
  endoLayout->addWidget(autoCheckBox);

  QHBoxLayout *mapLayout = new QHBoxLayout;
  mapLayout->addWidget(mapLabel);
  mapLayout->addWidget(mapLineEdit);
  mapLayout->addStretch();

  QGridLayout *imagesLayout = new QGridLayout;
  for (i = 0; i < Fn_MaxRank; i++) {
    
    imagesLayout->addWidget(imageLabel[i],i,0);
    imagesLayout->addWidget(imageLineEdit[i],i,1);
    imagesLayout->rowStretch(i);

  }
  imageGroupBox->setLayout(imagesLayout);

  QGridLayout *inverseLayout = new QGridLayout;
  for (i = 0; i < Fn_MaxRank; i++) {
    
    inverseLayout->addWidget(inverseLabel[i],i,0);
    inverseLayout->addWidget(inverseLineEdit[i],i,1);
    inverseLayout->rowStretch(i);

  }
  inverseGroupBox->setLayout(inverseLayout);
  inverseGroupBox->hide();

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(cancelButton);
  buttonLayout->addWidget(loadButton);
  buttonLayout->addWidget(dismissButton);

  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addLayout(mapLayout,0,0);
  mainLayout->addLayout(rankLayout,1,0);
  mainLayout->addLayout(endoLayout,2,0);
  mainLayout->addWidget(imageGroupBox,3,0);
  mainLayout->addWidget(noAutoLabel,4,0);
  mainLayout->addWidget(inverseGroupBox,3,1);
  mainLayout->addLayout(buttonLayout,5,0,1,2,Qt::AlignRight);
  setLayout(mainLayout);

  setWindowTitle(tr("Load Morphism"));
  setFixedHeight(sizeHint().height());

  // connections
  connect(rankComboBox,SIGNAL(currentIndexChanged(int)),
  	  this,SLOT(resizeImageGroupBox(int)));

  connect(imageRankComboBox,SIGNAL(currentIndexChanged(int)),
          this,SLOT(changeImageRegExpValidator(int)));

  connect(endoCheckBox,SIGNAL(stateChanged(int)),
	  this,SLOT(toggleEndomorphism(int)));

  connect(mapLineEdit,SIGNAL(textChanged(QString)),
	  this,SLOT(enableIsAutoLoadButtons()));

  for(i = 0; i < Fn_MaxRank; i++) {

    connect(imageLineEdit[i],SIGNAL(textChanged(QString)),
	    this,SLOT(enableIsAutoLoadButtons()));

  }
    
  connect(cancelButton,SIGNAL(clicked()),
	  this,SLOT(reject()));

  connect(loadButton,SIGNAL(clicked()),
	  this,SLOT(morphismLoad()));

  connect(dismissButton,SIGNAL(clicked()),
	  this,SLOT(close()));

  setAttribute(Qt::WA_DeleteOnClose);

}

// PRIVATE SLOTS

void MorphismDialog::changeImageRegExpValidator(int itemIndex) {

  // called when the image rank changes, this updates the RegExp
  // Validator for images
  
  imageBasis.changeRank(itemIndex + Fn_MinRank);
  rankRegExp->setRegExp(imageBasis.basisValidate());
  enableIsAutoLoadButtons();

}

void MorphismDialog::enableIsAutoLoadButtons() {
  
  // called when anything changes, this checks to see is if enough data
  // is present to load the morphism or check if it is an automorphism
  int i,rank,imageRank;
  bool enable = true;
  QString text;

  rank = rankComboBox->currentIndex() + Fn_MinRank;
  imageRank = imageRankComboBox->currentIndex() + Fn_MinRank;

  // test to see if there is a name for the function
  text = mapLineEdit->text();
  if (text.isEmpty()) enable = false;

  // test to see if the words are in the correct basis
  for (i = 0; i < rank; i++) {

    text = imageLineEdit[i]->text();
    if (text.isEmpty()) enable = false;
    if (!imageBasis.inBasis(text)) enable = false;

  }
  
  // test to see if the ranks are equal
  loadButton->setEnabled(enable);
  if (rank == imageRank)
    autoCheckBox->setEnabled(enable);
  else
    autoCheckBox->setEnabled(false);

}

void MorphismDialog::morphismLoad() {

    // called when the load button is clicked, this function loads the
    // morphism

    int i,rank = rankComboBox->currentIndex() + Fn_MinRank;
    int imageRank = imageRankComboBox->currentIndex() + Fn_MinRank;
    FnWord w[rank];
    FnMap phi(imageRank);
    FnMap phi_inv(imageRank);

    for (i = 0; i < rank; i++) {

        w[i] = imageLineEdit[i]->text();
        w[i].tighten();
        imageLineEdit[i]->setText(w[i]);
        imageLineEdit[i]->setEnabled(false);
        phi.insert(basis.at(2*i),w[i]);
        phi.insert(basis.at(2*i+1),w[i].inverse());

    }

    emit updateMorphismList(mapLineEdit->text(),phi);

    if (autoCheckBox->checkState() == Qt::Checked) {
        phi_inv = phi.isAutomorphism();
        if (!phi_inv) {
            noAutoLabel->show();
        } else {
            for (i = 0; i < rank; i++) {

                inverseLabel[i]->show();
                inverseLineEdit[i]->show();
                inverseLineEdit[i]->setText(phi_inv.value(basis.at(2*i)));
                inverseLineEdit[i]->setEnabled(false);

            }
            emit updateMorphismList(mapLineEdit->text() + "_Inv",phi_inv);
            inverseGroupBox->show();
            inverseGroupBox->updateGeometry();
        }
    }

    rankComboBox->setEnabled(false);
    imageRankComboBox->setEnabled(false);
    endoCheckBox->setEnabled(false);
    autoCheckBox->setEnabled(false);
    mapLineEdit->setEnabled(false);
    cancelButton->setEnabled(false);
    loadButton->hide();
    loadButton->setDefault(false);
    dismissButton->show();
    dismissButton->setDefault(true);
    imageGroupBox->updateGeometry();
    setFixedHeight(sizeHint().height());

}

void MorphismDialog::resizeImageGroupBox(int itemIndex) {

    int i;

    while (i < itemIndex + Fn_MinRank) {

        imageLabel[i]->show();
        imageLineEdit[i]->show();
        i++;

    }

    while (i < Fn_MaxRank) {

      imageLabel[i]->hide();
      imageLineEdit[i]->hide();
      i++;

    }

    // update rank of input basis
    basis.changeRank(itemIndex + Fn_MinRank);

    // if map is an endomorphism, change validator
    if (endoCheckBox->checkState() == Qt::Checked) {

      imageRankComboBox->setCurrentIndex(itemIndex);
      changeImageRegExpValidator(itemIndex);

    }

    imageGroupBox->updateGeometry();
    setFixedHeight(sizeHint().height());
    enableIsAutoLoadButtons();

}

void MorphismDialog::toggleEndomorphism(int state) {
  // inputs for image rank

  imageRankComboBox->setCurrentIndex(rankComboBox->currentIndex());
  imageBasis.changeRank(imageRankComboBox->currentIndex());
  changeImageRegExpValidator(imageRankComboBox->currentIndex());

  if (state == Qt::Unchecked) {

    imageRankLabel->show();
    imageRankComboBox->show();

  }

  if (state == Qt::Checked) {

    imageRankLabel->hide();
    imageRankComboBox->hide();

  }

  enableIsAutoLoadButtons();

}

