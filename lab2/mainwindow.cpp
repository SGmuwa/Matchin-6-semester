#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QException>


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

class IHaveHealthDamageName
{
public:
    IHaveHealthDamageName(){}
    // Получает жизни.
    float virtual getHealth(void) const = 0;
    // Получает урон.
    float virtual getDamage(void) const = 0;
    // Получает имя.
    QString virtual getName(void) const = 0;
    virtual ~IHaveHealthDamageName(){}
protected:
    // Устанавливает здоровье.
    void virtual setHealth(float damage) = 0;
    // Устанавливает урон.
    void virtual setDamage(float damage) = 0;
    // Устанавливает имя.
    void virtual setName(QString name) = 0;
};

class AbstractHaveHealthDamageName : public IHaveHealthDamageName
{
public:
    AbstractHaveHealthDamageName()
    {
        health = -1;
        damage = -1;
    }
    AbstractHaveHealthDamageName(QString name, float health, float damage)
    {
        this->setName(name);
        this->setHealth(health);
        this->setDamage(damage);
    }
    // Получает количество здоровья.
    float virtual getHealth(void) const override
    {
        return health;
    }
    // Получает количество урона.
    float virtual getDamage(void) const override
    {
        return damage;
    }
    // Получает имя.
    QString virtual getName(void) const override
    {
        return name;
    }
    virtual ~AbstractHaveHealthDamageName() override {}
protected:
    // Устанавливает количество здоровья.
    void setHealth(float health) override
    {
        this->health = health;
    }
    // Устанавливает количество урона
    void setDamage(float damage) override
    {
        this->damage = damage;
    }
    void setName(QString name) override
    {
        this->name = name;
    }
private:
    float health;
    float damage;
    QString name;
};

// Экипировка.
class ToolForWar : public AbstractHaveHealthDamageName
{
public:
    ToolForWar()
        : AbstractHaveHealthDamageName("NULL", -1, -1){ }
    ToolForWar(QString name, float addPercentHealth, float addPercentDamage)
        : AbstractHaveHealthDamageName(name, addPercentHealth, addPercentDamage) {}
    virtual ~ToolForWar() override {}
};

// Класс, представляющий атакующую единицу.
class AttackUnit : public AbstractHaveHealthDamageName
{
public:
    AttackUnit()
        : AbstractHaveHealthDamageName("NULL", -1, -1) {}
    AttackUnit(QString name, float health, float damage)
        : AbstractHaveHealthDamageName(name, health, damage) {}
    virtual ~AttackUnit() override {}
private:
};

// Боец. Юнит с оружием.
class Fighter : public IHaveHealthDamageName
{
public:
    Fighter(): IHaveHealthDamageName()
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
    virtual float getHealth() const override
    {
        return tool.getHealth() / 100.0f * unit.getHealth();
    }
    // Получаем атаку бойца.
    virtual float getDamage() const override
    {
        return tool.getDamage() / 100.0f * unit.getDamage();
    }
    // Получаем имя бойца.
    virtual QString getName() const override
    {
        return unit.getName() + tool.getName();
    }
    virtual ~Fighter() override {}
private:
    AttackUnit unit;
    ToolForWar tool;
    void setHealth(float) override{throw QException();}
    void setDamage(float) override{throw QException();}
    void setName(QString) override{throw QException();}
};

// Класс отвечает за одну из сторон битвы.
// IHaveHealthDamageName означает, что мы можем получить жизни и урон этой стороны.
class SideOfBattle : public IHaveHealthDamageName
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
    virtual float getHealth() const override
    {
        float output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getHealth();
        return output;
    }
    // Получает атаку стороны.
    virtual float getDamage() const override
    {
        float output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getDamage();
        return output;
    }
    virtual QString getName() const override
    {
        QString output;
        for(int i = 0; i < 3; i++)
            output += fighters[i].getName();
        return output;
    }
    const QList<AttackUnit> getAvalibleUnits() const
    {
        return avalibleUnits;
    }
    const QList<ToolForWar> getAvalibleTools() const
    {
        return avalibleTools;
    }
    virtual ~SideOfBattle() override{}
private:
    Fighter fighters[3] = {Fighter(), Fighter(), Fighter()};
    QList<AttackUnit> avalibleUnits;
    QList<ToolForWar> avalibleTools;
    void setHealth(float) override{throw QException();}
    void setDamage(float) override{throw QException();}
    void setName(QString) override{throw QException();}
};

// Класс отвечает за обе стороны битвы.
class Battle
{
public:
    Battle()
    {
        QList<AttackUnit> units;
        QList<ToolForWar> tools;
        units.append(AttackUnit("LEFT", 1, 1));
        tools.append(ToolForWar("TOOLLEFT", 1, 2));
        side.append(SideOfBattle(units, tools));
        units.clear();
        tools.clear();
        units.append(AttackUnit("RIGHT", 2, 2));
        tools.append(ToolForWar("TOOLRIGTH", 3, 4));
        side.append(SideOfBattle(units, tools));
    }
    // Возвращает идентификатор победителя. Если -1, то ничья.
    int whoWin()
    {
        int output = 0;
        bool isOdd = false; // true, если ничья.
        for(int i = output + 1; i < side.count(); i++)
            switch (whoWin(output, i)) {
            case 0:
                break;
            case -1:
                isOdd = true;
                /* break; do not need. */ [[clang::fallthrough]];
            case 1:
                output = i;
                break;
            }
        return isOdd ? -1 : output;
    }
private:
    // 0 - left, 1 - right, -1 - ничья.
    int whoWin(int leftIndex, int rightIndex) const
    {
        return whoWin(side.at(leftIndex), side.at(rightIndex));
    }
    // 0 - left, 1 - right, -1 - ничья.
    int whoWin(SideOfBattle left, SideOfBattle right) const
    {
        float healthLeft = left.getHealth() - right.getDamage();
        float healthRight = right.getHealth() - left.getDamage();
        return healthLeft > healthRight ? 0 : healthRight > healthLeft ? 1 : -1;
    }
    QList<SideOfBattle> side;
};

// Класс, который содержит поля интерфейса для заполнения юнита.
class VisualManagerUnit
{
public:
    VisualManagerUnit(MainWindow * mainWindow, QComboBox * comboBox, QLabel * labelHealth, QLabel * labelDamage, QList<IHaveHealthDamageName*> * avalible)
    {
        this->comboBox = comboBox;
        this->labelHealth = labelHealth;
        this->labelDamage = labelDamage;
        this->avalible = avalible;
        mainWindow->connect(comboBox, SIGNAL(currentIndexChanged(int)), mainWindow, SLOT(eventCurrentIndexChanged(int)));
        if(comboBox->count() != 0)
            throw new QException();
        for(IHaveHealthDamageName * elm : *avalible)
        {
            comboBox->addItem(elm->getName());
        }
    }
    void update(void) const{

    }
private:
    void eventCurrentIndexChanged(int index)
    {
        labelHealth->setNum((double)(avalible->at(index)->getHealth()));
        labelDamage->setNum((double)(avalible->at(index)->getDamage()));
    }
    QComboBox * comboBox;
    QLabel * labelHealth;
    QLabel * labelDamage;
    QList<IHaveHealthDamageName*> * avalible;
};
