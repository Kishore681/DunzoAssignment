#include <bits/stdc++.h>

using namespace std; 

// struct Ingredient to store {ingredient_name, ingredient_amount} 
struct Ingredient {

    string name; 
    int amount; 

    Ingredient(const string & ingredient_name, const int &ingredient_amount){
        this->name = ingredient_name; 
        this->amount =  ingredient_amount;
    }
    Ingredient(){

    }

}; 

// struct Beverage to store {beverage_name, {{ingredient_name,ingredient_req_amount},..... }} 
struct Beverage {

    string name; 
    unordered_map<string,int> ingredient_map; 


    Beverage(const string & beverage_name, const vector<Ingredient> & beverage_ingredients){
        this->name = beverage_name; 
        for(const Ingredient &ingredient: beverage_ingredients){
            ingredient_map[ingredient.name] = ingredient.amount;
        } 
    } 
    Beverage(){

    }

}; 



// included printing to stdout in methods for testing purposes and to show 
// the output to the interviewer.
// display_message flag is included  in some methods just for printing showing output purposes to interviewer.

class CoffeeMachine {

    // no_of_outlets of coffee_machine 
    int no_of_outlets;

    // stores all available Ingredients as {ingredient_name, quantity_of_ingredient}
    // useful for O(1) Ingredient quantity access. 
    unordered_map<string,int> available_ingredients_map; 

    

    vector<Beverage> all_beverages;
    // stores id (index) of beverages, for easy access,
    unordered_map<string,int> beverage_name_to_id;

    // both of the below variables can also be made non static 

    // quantity_limit which indicates some ingredient_quantity is running low. 
    static const int QUANTITY_LIMIT = 80;  

    // default refill amount, to refill any ingredients which are running low.
    // you can use refill() method given below. you can also pass any amount of refill_amount.
    static const int REFILL_AMOUNT = 30; 

public : 

    // default Constructor 
    CoffeeMachine(){

    }

    // parameterized Constructor
    CoffeeMachine(int no_of_outlets, vector<Ingredient>& lst_of_all_available_ingredients,
    vector<Beverage> lst_of_all_beverages){

        this->no_of_outlets = no_of_outlets;

        for(const auto ingredient : lst_of_all_available_ingredients){
            available_ingredients_map[ingredient.name] = ingredient.amount; 
        }

        
        int id =0; 
        for(const auto beverage : lst_of_all_beverages){
            all_beverages.push_back(beverage); 
            beverage_name_to_id[beverage.name] = id++;
        }

    }

    // returns list of all ingredients_name which are running low. (ingredient_quantity < Quantity_limit)
    vector<string> getListOfRunningLowIngredients (){
        vector<string> lowIngredients; 
        for(const pair<string, int>& p : available_ingredients_map){
            if (p.second < QUANTITY_LIMIT){
                lowIngredients.push_back(p.first); 
            }
        }
        return lowIngredients; 
    }

    // refills all ingredients which are running low 
    void RefillLowIngredients (int refill_amount = REFILL_AMOUNT, bool display_message = false){

        vector<string> lowIngredients = getListOfRunningLowIngredients(); 
        for(const string &ingredient : lowIngredients){
            available_ingredients_map[ingredient] += refill_amount;
        }
        if (display_message){
            cout << "Refilled All running low ingredients !!!! "<<"\n"; 
        }
    }

    // checks if it is possible to serve beverageName, by going through list of all availableIngredients
    bool checkIfServePossible (string beverage_name, bool display_message = false){
        int beverage_id = beverage_name_to_id[beverage_name];

        for(const auto ingredient : all_beverages[beverage_id].ingredient_map){
            if (!available_ingredients_map.count(ingredient.first)){
                if (display_message){
                    cout << beverage_name << " cannot be served because " << 
                    ingredient.first << " is not available"<<"\n";
                }
                return false; 
            }
            if (available_ingredients_map[ingredient.first] < ingredient.second){
                if (display_message){
                    cout << beverage_name << " cannot be served because required amount of " << 
                    ingredient.first << " is not available"<<"\n";
                }
                return false;
            }
        }
        return true;
    }

    // checks if it is possible to serve beverageName, if it is possible then it serves the beverageName, 

    bool serve(const string &beverage_name, bool display_message = false){

        if (!checkIfServePossible(beverage_name)){
            if (display_message){
                checkIfServePossible(beverage_name, true); 
                cout <<"\n";  
            }
            return false; 
        }
        int beverage_id = beverage_name_to_id[beverage_name]; 

        for(const auto ingredient : all_beverages[beverage_id].ingredient_map){
            available_ingredients_map[ingredient.first] -= ingredient.second;
        }

        if (display_message){
            cout << beverage_name << " has been served!"<<"\n"; 
        }

        return true;

    }


    // checks if you can serve list of beverages at a single time, 
    // it is possible to serve all the beverages given, then it will serve
    // else it wont serve any beverage.
    bool serve(vector<string> &beverage_names, bool display_message = false){

        if (display_message){
            cout << "Trying to serve " ; 
            for( string &beverage_name : beverage_names){
                cout << beverage_name << " ";
            }
            cout << "\n"; 
        }

        for(string beverage_name: beverage_names){
            if (!checkIfServePossible(beverage_name)){
                if (display_message){
                    cout << "Complete Serve Not Possible Because "; 
                    checkIfServePossible(beverage_name, true); 
                }
                return false;
            }
        }


        // serving beverages only if after checking if all beverages can be served. 
        for(string beverage_name: beverage_names){
            serve(beverage_name); 
        }

        cout << "Complete serve Possible, All beverages served!!"<< "\n";  

        return true;
    }

   

     // gives all possible parallel servings [every beverage repeats at most one time]
    // exponential complexity, only good for no_of_outlets < 20
    vector<vector<string>>  GetAllPossibleValidParallelServings (){

        vector<vector<string>> all_combinations; 

        int tmask = (1 << no_of_outlets);

        for(int mask = 1; mask < tmask; ++mask){
            vector<int> v; 
            for(int j = 0; j < no_of_outlets; ++j){
                if (mask & (1 << j)){
                    v.push_back(j); 
                }
            }

            vector<string> combination; 

            // as serving a beverage might change the ingredient amount 
            // and we are looking for all combinations. 
            // so we will reset ingredient_map to the initial value.


            auto copy_available_ingredients_map = available_ingredients_map;

            bool all_combination_serve_possible = true; 

            for(int ind : v){
                if (serve(all_beverages[ind].name)){
                    combination.push_back(all_beverages[ind].name); 
                }
                else{
                    all_combination_serve_possible = false; 
                }
            }

            if (!combination.empty() && all_combination_serve_possible){
                all_combinations.push_back(combination); 
            }



            available_ingredients_map = copy_available_ingredients_map; 
        }
        return all_combinations;
    }


    // method just for printing all possible parallelServings
    void printAllPossibleParallelServings (){

        vector<vector<string>>  allCombinations = GetAllPossibleValidParallelServings();
        int num = 1; 
        if (allCombinations.empty()){
            cout << "NO SERVINGS POSSIBLE" <<"\n";
        }
        for(const auto  &combination : allCombinations){
            cout << "Possible Parallel serving Combination number : "<< num++ << "\n"; 
            
            for(const string &beverage : combination){
                cout << beverage << " ";
            }
            cout << "\n"; 
            cout <<"------------------------------------------------" <<"\n";
        }
        
    }



}; 




void testCase (){

    /*

    I created input file link given below which has 10 test cases  
    // https://pastebin.com/hZQ7DCst 



    #### The only assumption I am doing is the number of beverages given in a testcase is  >= 3. 
    Because I am generating a random 3 beverages combination to test serve method in CoffeeMachine.

    ####  INPUT FORMAT ##### 



    T (no_test_cases)  (keep T = 1, if you want to give only one test case )

    N (no_of_outlets)

    M (total_no_of_available_ingredients)

    ingredient_name  ingredient_amount  -> repeat this M lines

    beverage_name                                        
    m (no_of_ingredient_required for the beverage_name)
    ingredient_name ingredient_amount 
    -> repeat above 3 lines N times 


    Example 
    // input taken from 
    // https://www.npoint.io/docs/77e0bf528e4af43cdc10
    // try to copy the below lines 315 to 352, run this program 
    // and paste it as input and see the output

    1
    4
    5
    hot_water  500
    hot_milk  500
    ginger_syrup  100
    sugar_syrup  100
    tea_leaves_syrup  100

    hot_tea  
    5
    hot_water  200
    hot_milk  100
    ginger_syrup  10
    sugar_syrup  10
    tea_leaves_syrup  30

    hot_coffee 
    5
    hot_water  100
    ginger_syrup  30
    hot_milk  400
    sugar_syrup  50
    tea_leaves_syrup  30

    black_tea  
    4
    hot_water  300
    ginger_syrup  30
    sugar_syrup  50
    tea_leaves_syrup  30

    green_tea 
    4
    hot_water 100
    ginger_syrup 30
    sugar_syrup 50
    green_mixture 30


    */



    int outlets;
    cin >> outlets; 

    int total_no_of_items ; 
    cin >> total_no_of_items; 

    vector<Ingredient> lst_of_all_ingredients (total_no_of_items); 

    for(auto &ingredient : lst_of_all_ingredients){
        cin >> ingredient.name >> ingredient.amount;  
    }   

    vector<Beverage> lst_of_all_beverages ; 

    string beverage_name ; 
    string ingredient_name;
    int ingredient_value; 
    int ingredient_count ; 

    for(int i= 0; i  < outlets; ++i){
        cin >> beverage_name;
        cin >> ingredient_count; 
        vector<Ingredient> ingredient_list; 
        for(int j = 0; j < ingredient_count; ++j){
            cin >>  ingredient_name >>  ingredient_value; 
            ingredient_list.push_back(Ingredient(ingredient_name, ingredient_value)); 
        }
        lst_of_all_beverages.push_back(Beverage(beverage_name, ingredient_list)); 
    }
    
    CoffeeMachine coffee_machine (outlets,lst_of_all_ingredients, lst_of_all_beverages); 


    // testing methods of coffee_machine 

    // printing valid possible parallel servings 
    coffee_machine.printAllPossibleParallelServings();

    cout <<"\n\n";

    // checking serve method which takes a single beverage to serve 
    coffee_machine.serve(lst_of_all_beverages.front().name, true); 
    cout <<"\n"; 
    coffee_machine.serve(lst_of_all_beverages.back().name, true); 
    cout << "\n";

    vector<string> toServe1 = {lst_of_all_beverages.front().name, lst_of_all_beverages[1].name, lst_of_all_beverages.back().name};
    vector<string> toServe2 = {lst_of_all_beverages.front().name, lst_of_all_beverages[(int)lst_of_all_beverages.size() - 2].name, lst_of_all_beverages.back().name};


    // checking serve method which takes list of beverages to serve.
    coffee_machine.serve(toServe1, true); 
    cout <<"\n\n"; 

    coffee_machine.serve(toServe2, true); 
    cout <<"\n\n";

    auto low_ingredients = coffee_machine.getListOfRunningLowIngredients(); 
    cout << "Ingredients running low are " ;
    if (low_ingredients.empty()){
        cout << "NONE" << "\n"; 
    }
    else{
        for(const auto &s : low_ingredients){
            cout << s <<" ";
        }
        cout <<"\n";
    }
    cout << "\n"; 




    coffee_machine.RefillLowIngredients(100, true); 
    
    cout << "--------------------------------------------------------" <<"\n"; 

}



// }

int main(){

    // no of test cases 

    int t;
    cin >> t; 

    for(int i = 1 ; i <= t; ++i){

        cout << "Test Case #"<< i << " Output" <<"\n"; 
        testCase();        
    }


    





    return 0; 
}