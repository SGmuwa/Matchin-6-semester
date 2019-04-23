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

class IHaveHealthDamageName
{
public:
    IHaveHealthDamageName(){}
    // Получает жизни.
    float virtual getHealth(void) const;
    // Получает урон.
    float virtual getDamage(void) const;
    // Получает имя.
    QString virtual getName(void) const;
    virtual ~IHaveHealthDamageName();
    IHaveHealthDamageName(const IHaveHealthDamageName&) = default; IHaveHealthDamageName(IHaveHealthDamageName&&) = default; IHaveHealthDamageName& operator=(const IHaveHealthDamageName&) = default; IHaveHealthDamageName& operator=(IHaveHealthDamageName&&) = default;
protected:
    // Устанавливает здоровье.
    void virtual setHealth(float damage);
    // Устанавливает урон.
    void virtual setDamage(float damage);
    // Устанавливает имя.
    void virtual setName(QString name);
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
    float getHealth(void) const override
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
    AbstractHaveHealthDamageName(const AbstractHaveHealthDamageName&) = default; AbstractHaveHealthDamageName(AbstractHaveHealthDamageName&&) = default; AbstractHaveHealthDamageName& operator=(const AbstractHaveHealthDamageName&) = default; AbstractHaveHealthDamageName& operator=(AbstractHaveHealthDamageName&&) = default;
    virtual ~AbstractHaveHealthDamageName() override;
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
        : AbstractHaveHealthDamageName("NULL", -1, -1){}
    ToolForWar(QString name, float addPercentHealth, float addPercentDamage)
        : AbstractHaveHealthDamageName(name, addPercentHealth, addPercentDamage) {}
    ToolForWar(const ToolForWar&) = default; ToolForWar(ToolForWar&&) = default; ToolForWar& operator=(const ToolForWar&) = default; ToolForWar& operator=(ToolForWar&&) = default;
    virtual ~ToolForWar();
private:
};

// Класс, представляющий атакующую единицу.
class AttackUnit : public AbstractHaveHealthDamageName
{
public:
    AttackUnit()
        : AbstractHaveHealthDamageName("NULL", -1, -1) {}
    AttackUnit(QString name, float health, float damage)
        : AbstractHaveHealthDamageName(name, health, damage) {}
    AttackUnit(const AttackUnit&) = default; AttackUnit(AttackUnit&&) = default; AttackUnit& operator=(const AttackUnit&) = default; AttackUnit& operator=(AttackUnit&&) = default;
    virtual ~AttackUnit();
private:
};

// Боец. Юнит с оружием.
class Fighter : public IHaveHealthDamageName
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
    float getHealth() const override
    {
        return tool.getHealth() / 100.0f * unit.getHealth();
    }
    // Получаем атаку бойца.
    float getDamage() const override
    {
        return tool.getDamage() / 100.0f * unit.getDamage();
    }
    Fighter(const Fighter&) = default; Fighter(Fighter&&) = default; Fighter& operator=(const Fighter&) = default; Fighter& operator=(Fighter&&) = default;
    virtual ~Fighter() override;
private:
    AttackUnit unit;
    ToolForWar tool;
};

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
    float getHealth() const override
    {
        float output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getHealth();
        return output;
    }
    // Получает атаку стороны.
    float getDamage() const override
    {
        float output = 0;
        for(int i = 0; i < 3; i++)
            output += fighters->getDamage();
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
    SideOfBattle(const SideOfBattle&) = default; SideOfBattle(SideOfBattle&&) = default; SideOfBattle& operator=(const SideOfBattle&) = default; SideOfBattle& operator=(SideOfBattle&&) = default;
    virtual ~SideOfBattle() override;
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
    VisualManagerUnit()
    {

    }
private:
    QComboBox * abs;
};


IHaveHealthDamageName::~IHaveHealthDamageName(){}
AbstractHaveHealthDamageName::~AbstractHaveHealthDamageName(){}
ToolForWar::~ToolForWar(){}
AttackUnit::~AttackUnit(){}
Fighter::~Fighter(){}
SideOfBattle::~SideOfBattle(){}
