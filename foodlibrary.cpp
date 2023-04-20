#include "foodLibrary.h"
#include<QDebug>
FoodLibrary::FoodLibrary()
{
    initialIngredients();
    initialRecipes();

    //Andy Tran Added
    Recipe recipe;
    QHash<Ingredient*, int> myHash1;
    myHash1.insert(broccoli, 1);
    myHash1.insert(tomato, 2);
    myHash1.insert(peanut, 3);

    QHash<Ingredient*, int> myHash2;
    myHash2.insert(broccoli, 1);
    myHash2.insert(tomato, 2);
    myHash2.insert(peanut, 3);

    QHash<Ingredient*, int> myHash3;
    myHash3.insert(broccoli, 1);
    myHash3.insert(tomato, 2);
    myHash3.insert(peanut, 3);
    Step step1 = Step(Action::CUT, myHash1);
    Step step2 = Step(Action::MIX, myHash2);
    Step step3 = Step(Action::BOIL, myHash3);
    QList<Step> stepVector = {step1,step2,step3};
    recipe = Recipe(stepVector);

    recipe.printSteps();
}

void FoodLibrary::initialIngredients()
{
    allIngredients.push_back(broccoli);
    allIngredients.push_back(peanut);
    allIngredients.push_back(tomato);
    allIngredients.push_back(beef);
    allIngredients.push_back(pork);
    allIngredients.push_back(chicken);
    allIngredients.push_back(pepper);
    allIngredients.push_back(soysauce);
    allIngredients.push_back(riceNoodles);
    allIngredients.push_back(carrot);
    allIngredients.push_back(cabbage);
}

void FoodLibrary::initialRecipes()
{
    QJsonObject obj;
    Recipe pho;
    obj = Recipe::readJsonFile(":/sprites/recipes/Pho.txt");
    pho = Recipe::deserialize(obj);

    qDebug()<<pho.getName();
    qDebug()<<pho.getDifficulty();
    qDebug()<<pho.getDescription();
    qDebug()<<pho.getLearnMore();
    qDebug()<<"\nIngredients: ";
    qDebug()<<"Name   cut    mix    boil    fired ";
    QHash<Ingredient*, int> ingredients = pho.getIngredients();
    QList<Ingredient*> iList = ingredients.keys();
    for(Ingredient* i : iList){
        qDebug()<<i->getName()<<i->getIsCut()<<i->getIsMixed()
               <<i->getIsBoiled()<<i->getIsFried();

    }


    qDebug()<<"\nPrep Steps: ";
    QMap<int,Ingredient*> prep = pho.getPrepSteps();
    QList<Ingredient*> prepList = prep.values();
    for(Ingredient* p:prepList ){
         qDebug()<<p->getName()<<" Cut: "<<p->getIsCut();
    }

    qDebug()<<"\nCooking Steps: ";
    QMap<int,Ingredient*> cook = pho.getCookingSteps();
    QList<Ingredient*> cookList = cook.values();
    for(Ingredient* c:cookList ){
         qDebug()<<c->getName()<<" Cooked: "<<c->getIsBoiled();
    }

}
