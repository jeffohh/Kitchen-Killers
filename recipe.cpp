#include "recipe.h"

Recipe::Recipe()
{

}

Recipe::Recipe(QString name, int difficulty, QHash<Ingredient *, int> ingredients,
               QMap<int, Ingredient *> prepSteps, QMap<int, Ingredient *> cookingSteps):
    name(name),
    difficulty(difficulty),
    ingredients(ingredients),
    prepSteps(prepSteps),
    cookingSteps(cookingSteps)
{

}

QHash<Ingredient *, int> Recipe::getIngredients() const
{
    return ingredients;
}

QMap<int, Ingredient *> Recipe::getPrepSteps() const
{
    return prepSteps;
}

QMap<int, Ingredient *> Recipe::getCookingSteps() const
{
    return cookingSteps;
}

QJsonObject Recipe::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open JSON file:" << file.errorString();
        return QJsonObject();
    }

    QTextStream stream(&file);
    QString jsonString = stream.readAll();
    //qDebug()<<jsonString;
    file.close();

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    if (jsonDoc.isNull()) {
        qDebug() << "Failed to parse JSON file:" << error.errorString();
        return QJsonObject();
    }

    if (!jsonDoc.isObject()) {
        //qDebug() << "JSON file does not contain a valid JSON object:" << jsonString;
        return QJsonObject();
    }

    return jsonDoc.object();

}

Recipe Recipe::deserialize(const QJsonObject &jsonObj)
{
    QString name;
    int difficulty;
    QHash<Ingredient*, int> ingredients;
    QMap<int, Ingredient*> prepSteps;
    QMap<int, Ingredient*> cookingSteps;

    name = jsonObj["name"].toString();
    difficulty = jsonObj["difficulty"].toInt();

    // Deserialize ingredients
    QJsonObject ingredientsJson = jsonObj["ingredients"].toObject();
    for (const QString &ingredientName : ingredientsJson.keys()) {
        QJsonObject ingredientJson = ingredientsJson[ingredientName].toObject();
        int amount = ingredientJson["amount"].toInt();
        FoodCategory category = static_cast<FoodCategory>(ingredientJson["category"].toInt());
        QJsonArray tagsJson = ingredientJson["tags"].toArray();
        QVector<FoodTags> tags;
        for (const QJsonValue &tagJson : tagsJson) {
            tags.append(static_cast<FoodTags>(tagJson.toInt()));
        }

        Ingredient *ingredient = new Ingredient(ingredientName, category, tags);
        ingredients.insert(ingredient, amount);
    }

    // Deserialize prep steps
    QJsonArray prepStepsJson = jsonObj["prepSteps"].toArray();
    for (const QJsonValue &stepJson : prepStepsJson) {
        QJsonObject stepObj = stepJson.toObject();
        int order = stepObj["order"].toInt();
        QString ingredientName = stepObj["ingredientName"].toString();
        bool isCut = stepObj["isCut"].toBool();
        bool isMixed = stepObj["isMixed"].toBool();
        bool isBoiled = stepObj["isBoiled"].toBool();
        bool isFried = stepObj["isFried"].toBool();

        Ingredient *ingredient = new Ingredient();
        QList<Ingredient*> allPrep = ingredients.keys() ;
        for(Ingredient* ingr : allPrep)
        {
            if (ingr->getName() == ingredientName) {
                *ingredient = *ingr;
                break;
            }
        }
        if (ingredient) {
            prepSteps.insert(order, ingredient);
            ingredient->setIsCut(isCut);
            ingredient->setIsMixed(isMixed);
            ingredient->setIsBoiled(isBoiled);
            ingredient->setIsFried(isFried);
        }
    }

    // Deserialize cooking steps
    QJsonArray cookingStepsJson = jsonObj["cookingSteps"].toArray();
    for (const QJsonValue &cookJson : cookingStepsJson) {
        QJsonObject cookObj = cookJson.toObject();
        int order = cookObj["order"].toInt();
        QString ingredientName = cookObj["ingredientName"].toString();
        bool isCut = cookObj["isCut"].toBool();
        bool isMixed = cookObj["isMixed"].toBool();
        bool isBoiled = cookObj["isBoiled"].toBool();
        bool isFried = cookObj["isFried"].toBool();

        Ingredient *ingredient = new Ingredient();
        QList<Ingredient*> allCook = ingredients.keys();
        for(Ingredient* ingr: allCook)
        {
            if (ingr->getName() == ingredientName) {
                *ingredient = *ingr;
                break;
            }
        }

        if (ingredient) {
            cookingSteps.insert(order, ingredient);
            ingredient->setIsCut(isCut);
            ingredient->setIsMixed(isMixed);
            ingredient->setIsBoiled(isBoiled);
            ingredient->setIsFried(isFried);
        }
    }

    Recipe recipe;
    recipe.setName(name);
    recipe.setDifficulty(difficulty);
    recipe.setIngredients(ingredients);
    recipe.setPrepSteps(prepSteps);
    recipe.setCookingSteps(cookingSteps);
    return recipe;

}

void Recipe::setName(const QString &newName)
{
    name = newName;
}

void Recipe::setDifficulty(int newDifficulty)
{
    difficulty = newDifficulty;
}

void Recipe::setIngredients(const QHash<Ingredient *, int> &newIngredients)
{
    ingredients = newIngredients;
}

void Recipe::setPrepSteps(const QMap<int, Ingredient *> &newPrepSteps)
{
    prepSteps = newPrepSteps;
}

void Recipe::setCookingSteps(const QMap<int, Ingredient *> &newCookingSteps)
{
    cookingSteps = newCookingSteps;
}
