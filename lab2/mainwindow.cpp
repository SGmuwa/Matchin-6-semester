#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Экипировка.
class ToolForWar
{
public:
    ToolForWar()
    {
        *this = ToolForWar("NULL", -1, -1);
    }
    ToolForWar(QString name, int addPercentHealth, int addPercentDamage)
    {
        this->addPercentHealth = addPercentHealth;
        this->addPercentDamage = addPercentDamage;
        this->name = name;
    }
    // Получает количество бонуса: добавление в процентах к здоровью.
    int getAddPercentHealth()
    {
        return addPercentHealth;
    }
    // Получает количество бонуса: добавление в процентах к урону.
    int getAddPercentDamage()
    {
        return addPercentDamage;
    }
    QString getName()
    {
        return name;
    }
private:
    // Добавляет в процентах к здоровью
    int addPercentHealth;
    // Добавляет в процентах к урону.
    int addPercentDamage;
    // Имя экипировки.
    QString name;
};

// Класс, представляющий атакующую единицу.
class AttackUnit
{
public:
    AttackUnit()
    {
        this->name = "NULL";
        this->attack = -1;
        this->health = -1;
    }
    AttackUnit(QString name, int attack, int health)
    {
        this->name = name;
        this->attack = attack;
        this->health = health;
    }
    // Получить атаку юнита.
    int getAttack() {return attack;}
    // Получить оборону юнита.
    int getHealth() {return health;}
    // Получить имя юнита.
    QString getName() {return name;}
private:
    // Атака.
    int attack;
    // Здоровье.
    int health;
    // Имя юнита.
    QString name;
};

// Боец. Юнит с оружием.
class Fighter
{
public:
    Fighter()
    {
    }
    // Меняем бойца.
    void setUnit(AttackUnit unit)
    {
        this->unit = unit;
    }
    // Меняем его экипировку.
    void setTool(ToolForWar tool)
    {
        this->tool = tool;
    }
    // Получаем жизни пойца.
    float getHealth()
    {
        return tool.getAddPercentHealth() / 100.0f * unit.getHealth();
    }
    // Получаем атаку бойца.
    float getAttack()
    {
        return tool.getAddPercentDamage() / 100.0f * unit.getAttack();
    }
private:
    AttackUnit unit;
    ToolForWar tool;
};

class SideOfBattle
{
public:
    SideOfBattle(AttackUnit avalibleUnits[], size_t avalibleUnits_size, ToolForWar avalibleTools[], size_t avalibleTools_size)
    {
        for(size_t i = 0; i < avalibleUnits_size; i++)
            this->avalibleUnits.append(avalibleUnits[i]);
        for(size_t i = 0; i < avalibleTools_size; i++)
            this->avalibleTools.append(avalibleTools[i]);
    }
    SideOfBattle(QList<AttackUnit> avalibleUnits, QList<ToolForWar> avalibleTools)
    {
        for(AttackUnit unit : avalibleUnits)
            this->avalibleUnits.append(unit);
        for(ToolForWar tool : avalibleTools)
            this->avalibleTools.append(tool);
    }
    void setUnit(int unitType, int position)
    {
        fighters[position].setUnit(avalibleUnits.at(unitType));
    }
    void setTool(int toolType, int position)
    {
        fighters[position].setTool(avalibleTools.at(toolType));
    }
    // Получает здоровье стороны.
    float getHealth()
    {
        float output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getHealth();
        return output;
    }
    // Получает атаку стороны.
    float getAttack()
    {
        float output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getAttack();
        return output;
    }
    const QList<AttackUnit> getAvalibleUnits()
    {
        return avalibleUnits;
    }
    const QList<ToolForWar> getAvalibleTools()
    {
        return avalibleTools;
    }
private:
    Fighter fighters[3] = {Fighter(), Fighter(), Fighter()};
    QList<AttackUnit> avalibleUnits;
    QList<ToolForWar> avalibleTools;
};

class Battle
{
public:
    Battle()
    {

    }
private:
    SideOfBattle side[2] = {SideOfBattle(), SideOfBattle()};
};
