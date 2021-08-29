#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>

bool string_equal(const std::string &a, const std::string &b) //checks if strings are equal
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b)
                      { return tolower(a) == tolower(b); });
}

void play_game(std::string user_name, int balance) //plays the game
{
    std::cout << "<=============================================RULES=============================================>\n1. You bet an amount of money\n2. We give you a hint number and we ask you if the number is \"Higher\", \"Lower\", or \"Jackpot\"\n3. If you guess the number correct, you will get a multiple of the number you bet\n";
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
    while (true)
    {
        std::cin >> user_input;
        if (string_equal(user_input, "higher") && hidden_number > hint_number || string_equal(user_input, "lower") && hidden_number < hint_number || string_equal(user_input, "jackpot") && hidden_number == hint_number) //if user answer is correct(ignores case sensitivity)
        {
            balance = balance + balance * betting_amount;
        }
        else
        {
            balance = balance - betting_amount;
        }
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
    else
    {
        std::cout << "you can play the game bro\n";
    }
    play_game(user_name, balance);
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