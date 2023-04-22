#include "s1Pantry.h"
#include "ingredientbutton.h"
#include "ui_s1Pantry.h"

#include "ingredient.h"

s1Pantry::s1Pantry(Model& model,QWidget *parent) :
    QWidget(parent),
    m_model(model),
    ui(new Ui::s1Pantry)
{
    ui->setupUi(this);
    connect(ui->nextBtn, &QPushButton::clicked, this, &s1Pantry::nextPage);

    // initialize Pantry
    class FoodLibrary f;

    QList<ScrollCarousel*> pantryList;

    for (int i = MEAT; i <= STAPLE; i++) {
        FoodCategory food = static_cast<FoodCategory>(i);

        QGroupBox *box = new QGroupBox();
        box->setStyleSheet("QGroupBox {border: 0px;};");
        box->setFixedSize(675, 150);
        //box->setStyleSheet("QGroupBox {background-color: white};");
        QHBoxLayout *layout = new QHBoxLayout(box);

        switch (food) {
        case MEAT:
            box->setTitle("Meat");
            break;
        case VEGETABLE:
            box->setTitle("Vegetable");
            break;
        case CONDIMENT:
            box->setTitle("Condiment");
            break;
        case STAPLE:
            box->setTitle("Staple");
            break;
        default:
            break;
        }

        ScrollCarousel *pantry = new ScrollCarousel(true);       
        pantry->setStyleSheet("ScrollCarousel > QWidget > QWidget {background-color: transparent};");
        pantry->setFixedSize(650, 100);
        layout->addWidget(pantry);

        pantryList.append(pantry);


        QTimer::singleShot(0, this, [this, box, f, pantry, food] {
            for (Ingredient *ingredient : f.getAllIngredients()) {
                if (ingredient->getCate() == food) {
                    IngredientButton *button = new IngredientButton(*ingredient, 75);
                    //Set connection btw button and model
                    connect(button, &IngredientButton::onSelectedListUpdate, &m_model, &Model::onSelectedListUpdate);
                    pantry->addWidget(button);
                }
            }
            ui->pantryCategory->addWidget(box);
        });
    }


    // create filters
    auto is_not_peanut = [](QWidget* widget) {
        IngredientButton *button = dynamic_cast<IngredientButton*>(widget);
        if (button) {
            Ingredient ingredient = button->getIngredient();
            return !ingredient.getTags().contains(NUT);
        }

        return true; // if we cast, but not correct button then default true
    };

    auto is_vegan = [](QWidget* widget) {
        IngredientButton *button = dynamic_cast<IngredientButton*>(widget);
        if (button) {
            Ingredient ingredient = button->getIngredient();
            return ingredient.getTags().contains(VEGAN);
        }

        return true; // if we cast, but not correct button then default true
    };


    // connect checkboxes to filters
    connect(ui->allergyBox, &QCheckBox::stateChanged, this, [this, pantryList, is_not_peanut]() {
        bool checked = ui->allergyBox->isChecked();
        if (checked) {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->addFilter(is_not_peanut);
            }
        } else {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->removeFilter(is_not_peanut);
            }
        }
    });

    connect(ui->veganBox, &QCheckBox::stateChanged, this, [this, pantryList, is_vegan]() {
        bool checked = ui->veganBox->isChecked();
        if (checked) {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->addFilter(is_vegan);
            }
        } else {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->removeFilter(is_vegan);
            }
        }
    });

    //Set connection from s1 to Model -> move to step 2 and send the selected list
    connect(this, &s1Pantry::onSendS2SelectedIngredients, &m_model, &Model::onSendS2SelectedIngredients);
}

s1Pantry::~s1Pantry()
{
    delete ui;
}

void s1Pantry::nextPage()
{
    emit goToPage2();
    emit onSendS2SelectedIngredients();
}
