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
    int getHealth()
    {
        return (int)(tool.getAddPercentHealth() / 100.0f * unit.getHealth());
    }
    // Получаем атаку бойца.
    int getAttack()
    {
        return (int)(tool.getAddPercentDamage() / 100.0f * unit.getAttack());
    }
private:
    AttackUnit unit;
    ToolForWar tool;
};

class SideOfBattle
{
public:
    void setUnit(AttackUnit unit, int position)
    {
        fighters[position].setUnit(unit);
    }
    void setTool(ToolForWar tool, int position)
    {
        fighters[position].setTool(tool);
    }
    // Получает здоровье стороны.
    int getHealth()
    {
        int output = 0;
        for(int i = 0; i < 3; i++)
            output+= fighters->getHealth();
        return output;
    }
    // Получает атаку стороны.
    int getAttack()
    {
        int output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getAttack();
        return output;
    }
private:
    Fighter fighters[3] = {Fighter(), Fighter(), Fighter()};

};


