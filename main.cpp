#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>

void win_game(int &balance, int betting_amount, double win_multiplier, std::string user_name, std::string password) //normal winning game, where the users balance is increased based on the betting amount
{
    balance = balance + (betting_amount * win_multiplier);
    std::cout << "Well done, you got it right, you won: " << betting_amount * win_multiplier << " and your new balance is: " << balance << "\n";
    std::ofstream user_details1(user_name); //creates a new file for the user
    user_details1 << user_name << "\n" << password << "\n"<< balance;
     user_details1.close();
}
void win_jackpot_game(int &balance, int betting_amount, double win_multiplier, std::string user_name, std::string password) //adds the super big multiplier amount to the user's balance
{
    balance = balance + (betting_amount * win_multiplier * win_multiplier * 20);
    std::cout << "Well done, you got it right, you won: " << betting_amount * win_multiplier * win_multiplier * 20 << " and your new balance is: " << balance << "\n"; //multiplier is massive as it is a 1/100 chance

    std::ofstream user_details1(user_name); //creates a new file for the user
    user_details1 << user_name << "\n"
                  << password << "\n"
                  << balance;
}
void lose_game(int &balance, int betting_amount, double win_multiplier, std::string user_name, std::string password) //the user loses the game
{
    balance = balance - betting_amount;
    std::cout << "Oh no, you got it wrong, you lost: " << betting_amount << " and your new balance is: " << balance << "\n";
    std::ofstream user_details1(user_name); //creates a new file for the user
    user_details1 << user_name << "\n"
                  << password << "\n"
                  << balance;
}
bool string_equal(const std::string &a, const std::string &b) //checks if strings are equal
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b)
                      { return tolower(a) == tolower(b); });
}

void play_highlow_game(std::string user_name, std::string password, int balance) //plays the game
{
    std::cout << "<=============================================RULES=============================================>\n1. You bet an amount of money\n2. We give you a hint number and we ask you if the number is \"Higher\", \"Lower\", or \"Jackpot\"\n3. If you guess the number correct, you will get a multiple of the number you bet (if you get jackpot right, you will get a LARGE multiple)\n";
    srand(time(NULL));                    //sets the seed
    int hidden_number = rand() % 100 + 1; //hidden number
    int hint_number = rand() % 100 + 1;   //hint number
    int betting_amount;
    double win_multiplier = (double)rand() / (RAND_MAX + 1) * (2 - 0.5) + 0.5; //winning multiplier, should generate a decimal between 0.5 and 2
    std::string user_input;
    std::cout << "Please enter a betting amount, your balance is " << balance << "\n";
    while (true)
    {
        std::cin >> betting_amount;
        if (betting_amount > balance) //if betting amount is more than balance
        {
            std::cout << "you can't bet more than you own, try again\n";
        }
        else
        {
            break;
        }
    }
    std::cout << "Guess if the number is higher or lower or the same as " << hint_number << "\n Please enter \"Higher\" or \"Lower\" or \"Jackpot\"\n";
    std::cout << "the hidden number is" << hidden_number << "and the multiplier is" << win_multiplier << "\n";
    while (true) //this makes the user enter higher or lower
    {
        std::cin >> user_input;
        if (string_equal(user_input, "higher"))
        {
            if (hidden_number > hint_number) //if user was correct
            {
                win_game(balance, betting_amount, win_multiplier, user_name, password); //win game
                break;
            }
            else
            {
                lose_game(balance, betting_amount, win_multiplier, user_name, password); //lose game
                break;
            }
        }
        else if (string_equal(user_input, "lower"))
        {
            if (hidden_number < hint_number) //if user was correct
            {
                win_game(balance, betting_amount, win_multiplier, user_name, password); //win game
                break;
            }
            else //if user was wrong
            {
                lose_game(balance, betting_amount, win_multiplier, user_name, password); //lose game
                break;
            }
        }
        else if (string_equal(user_input, "jackpot"))
        {
            if (hidden_number == hint_number) //if user was correct
            {
                win_jackpot_game(balance, betting_amount, win_multiplier, user_name, password);
                break;
            }
            else
            {
                lose_game(balance, betting_amount, win_multiplier, user_name, password); //lose game
                break;
            }
        }
        else
        {
            std::cout << "Nope, that is not an acceptable response, please check what you wrote and retype your answer\n";
        }
        /*       if (string_equal(user_input, "higher") && hidden_number > hint_number || string_equal(user_input, "lower") && hidden_number < hint_number || string_equal(user_input, "jackpot") && hidden_number == hint_number) //if user answer is correct(ignores case sensitivity)
        {
            balance = balance + balance * betting_amount;
        }
        else
        {
            balance = balance - betting_amount;
        } */
    }
    if (balance < 100)//if balance is less than 100, the user is forced to exit
    {
        std::cout << "Your balance is too low, I'm gonna have to kick you out\n";
        exit;
    } 
    else
    {
        std::cout << "Success\n";
    }
}
void check_balance(std::string user_name, std::string password) //checks the balance of the user, and if the user has a balance less than 100, they automatically get 1000 coins to play with
{
    std::string foo;                       //garbage variable for the username and password
    int balance;                           //stores balance
    std::ifstream user_details(user_name); //opens the user name file
    user_details >> foo;                   //username
    user_details >> foo;                   //password
    user_details >> balance;               //balance
    if (balance < 100)
    {
        std::cout << "you broke bro, here I'll reset your balance to 1000\n";
        std::ofstream user_details1(user_name); //creates a new file for the user
        balance = 1000;
        user_details1 << user_name << "\n"
                      << password << "\n1000"; //rewrites everything
    }
    play_highlow_game(user_name, password, balance);
}

void registration_process(std::vector<std::string> unnacceptable_words)
{
    bool exit_loop = false;
    std::string username;
    std::string password;
    do //is stuck in a loop until the inputs are acceptable
    {
        char variablecatcher = 0;                                           //is used to catch an exception in the username
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clears cin cooldown, as the program was skipping over this line
        std::cout << "\nUsername: ";
        std::getline(std::cin, username); //gets username
        std::cout << "\nPassword: ";
        std::getline(std::cin, password); //gets password
        for (char i = 0; i < unnacceptable_words.size(); i++)
        {
            if (username.find(unnacceptable_words[i]) != std::string::npos) //sees if there are unnacceptable sequences in username
            {
                std::cout << "the \"" << unnacceptable_words[i] << "\" is not allowed in username, please press enter to try again\n";
                variablecatcher = 1;
            }
            if (password.find(unnacceptable_words[i]) != std::string::npos) //sees if there are unnacceptable sequences in username
            {
                std::cout << "the \"" << unnacceptable_words[i] << "\" is not allowed in password, please press enter to try again\n";
                variablecatcher = 1;
            }
        }
        if (username == "" || password == "")
        {
            std::cout << "I see what you did there you sneaky boy, press enter to try again\n";
            variablecatcher = 1;
        }
        variablecatcher == 0 ? exit_loop = true : exit_loop = false;
    } while (exit_loop == false);
    std::cout << "congrats, you have been registered\n";
    std::ofstream file(username); //creates new file for every user that is registered
    file << username << "\n"
         << password << "\n0"; //add username and password to file and gives them the starting balance of 0
}

void login_process()
{
    std::string username, password;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clears cin cooldown, as the program was skipping over this line
    std::cout << "\nUsername: ";
    std::getline(std::cin, username); //gets username
    std::cout << "\nPassword: ";
    std::getline(std::cin, password); //gets password
    std::ifstream file(username);     //opens file with username login
    std::string user, pass;           //used to get data from the file
    std::getline(file, user);         //gets username on first line
    std::getline(file, pass);         //gets password on next line
    if (user == username && pass == password)
    {
        std::cout << "you have logged in, welcome " << username << "\n";
        check_balance(username, password); //checks the balance of the username (felt it would be better to do it here instead of giving the password variable to the play_game function)
    }
    else
    {
        std::cout << "invalid username or password\n";
    }
}

int main()
{
    while (true)
    {
        std::cout << "1. Login\n2. Register\n"; //gives options for choices
        std::string choice;                     //will recieve choice, would have made integer, but string provides for any input
        std::cout << "your choice: ";
        std::cin >> choice; //recieves choice
        std::vector<std::string> unnacceptable_words = {" ", "_", "*", "&", "=", "'", "-", "\"", "-", "+", ",", "<", ">", ".", "@"};
        if (choice == "1")
        {
            login_process();
        }
        else if (choice == "2")
        {
            registration_process(unnacceptable_words);
        }
        else
        {
            std::cout << "\ninvalid statement\n";
        }
    }
}