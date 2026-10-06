#include "mywindow.h"
#include "ui_mywindow.h"

#include "FichierUtilisateur.h"

MyWindow::MyWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    // Configuration de la table des clients (ne pas modifier)
    ui->tableWidgetClients->setColumnCount(2);
    ui->tableWidgetClients->setRowCount(0);
    QStringList labelsTableClients;
    labelsTableClients << "Nom" << "Hash du mot de passe";
    ui->tableWidgetClients->setHorizontalHeaderLabels(labelsTableClients);
    ui->tableWidgetClients->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidgetClients->horizontalHeader()->setVisible(true);
    ui->tableWidgetClients->horizontalHeader()->setDefaultSectionSize(300);
    ui->tableWidgetClients->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetClients->verticalHeader()->setVisible(false);
    ui->tableWidgetClients->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    /// Exemples d'utilisation (à supprimer)
    setResultat(" ---- Bonjour !!! ---- ");
    ajouteTupleTableUtilisateurs("wagner",10);
}

MyWindow::~MyWindow()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles : ne pas modifier /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::setNom(const char* Text)
{
  if (strlen(Text) == 0 )
  {
    ui->lineEditNom->clear();
    return;
  }
  ui->lineEditNom->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* MyWindow::getNom()
{
  strcpy(_nom,ui->lineEditNom->text().toStdString().c_str());
  return _nom;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::setMotDePasse(const char* Text)
{
  if (strlen(Text) == 0 )
  {
    ui->lineEditMotDePasse->clear();
    return;
  }
  ui->lineEditMotDePasse->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* MyWindow::getMotDePasse()
{
  strcpy(_motDePasse,ui->lineEditMotDePasse->text().toStdString().c_str());
  return _motDePasse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MyWindow::isNouveauChecked()
{
  if (ui->checkBoxNouveau->isChecked()) return 1;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::setResultat(const char* Text)
{
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat->clear();
    return;
  }
  ui->lineEditResultat->setText(Text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des utilisateurs (ne pas modifier) //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::ajouteTupleTableUtilisateurs(const char* nom,int hash)
{
    char Hash[20];
    sprintf(Hash,"%d",hash);

    // Ajout
    int nbLignes = ui->tableWidgetClients->rowCount();
    nbLignes++;
    ui->tableWidgetClients->setRowCount(nbLignes);
    ui->tableWidgetClients->setRowHeight(nbLignes-1,10);

    QTableWidgetItem *item = new QTableWidgetItem;
    auto flags = item->flags();
    flags.setFlag(Qt::ItemIsEditable, false).setFlag(Qt::ItemIsSelectable, false);
    item->setFlags(flags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(nom);
    ui->tableWidgetClients->setItem(nbLignes-1,0,item);

    item = new QTableWidgetItem;
    flags = item->flags();
    flags.setFlag(Qt::ItemIsEditable, false).setFlag(Qt::ItemIsSelectable, false);
    item->setFlags(flags);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Hash);
    ui->tableWidgetClients->setItem(nbLignes-1,1,item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::videTableUtilisateurs()
{
    ui->tableWidgetClients->setRowCount(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les boutons ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::on_pushButtonLogin_clicked()
{
    const char* nom = getNom();
    const char* mdp = getMotDePasse();
    int nouveau = isNouveauChecked();

    int position = estPresent(nom);

    if (nouveau == 1) { // Cas 1 : On veut créer un nouvel utilisateur
        if (position > 0) {
            setResultat("Utilisateur déjà existant !");
        } else {
            ajouteUtilisateur(nom, mdp);
            setResultat("Nouvel utilisateur créé : bienvenue !");
        }
    } else { // Cas 2 : On veut se connecter à un compte existant
        if (position > 0) {
            int verif = verifieMotDePasse(position, mdp);
            if (verif == 1) {
                setResultat("Re-bonjour cher utilisateur !");
            } else {
                setResultat("Mot de passe incorrect...");
            }
        } else {
            setResultat("Utilisateur inconnu...");
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyWindow::on_pushButtonAfficheFichier_clicked()
{
    // 1. On vide la table avant de la remplir
    videTableUtilisateurs();

    // 2. On crée un tableau suffisamment grand pour accueillir les données
    UTILISATEUR utilisateurs[100]; 

    // 3. On récupère les utilisateurs depuis le fichier
    int nbUtilisateurs = listeUtilisateurs(utilisateurs);

    // 4. On ajoute chaque utilisateur trouvé dans la table graphique
    for (int i = 0; i < nbUtilisateurs; i++) {
        ajouteTupleTableUtilisateurs(utilisateurs[i].nom, utilisateurs[i].hash);
    }
}
