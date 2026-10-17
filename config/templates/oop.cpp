/**
 * ==============================================================================
 * ByteC: Starter Template: C++20 Object-Oriented Programming (Bank Account Lab)
 * ==============================================================================
 */

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

class BankAccount
{
protected:
    std::string ownerName;
    int accountNumber;
    double balance;

public:
    BankAccount(std::string owner, int accNum, double initialBalance)
        : ownerName(std::move(owner)), accountNumber(accNum), balance(initialBalance)
    {
    }

    virtual ~BankAccount() = default;

    void deposit(double amount)
    {
        if (amount <= 0.0)
        {
            std::cout << "Deposit amount must be positive.\n";
            return;
        }
        balance += amount;
        std::cout << "Deposited " << std::fixed << std::setprecision(2) << amount << "\n";
    }

    virtual bool withdraw(double amount)
    {
        if (amount <= 0.0 || amount > balance)
        {
            std::cout << "Insufficient funds or invalid withdrawal amount.\n";
            return false;
        }
        balance -= amount;
        std::cout << "Withdrew " << std::fixed << std::setprecision(2) << amount << "\n";
        return true;
    }

    virtual void applyInterest()
    {
        std::cout << "Standard account: no interest applied.\n";
    }

    virtual void displaySummary() const
    {
        std::cout << "Account #" << accountNumber
                  << " | Owner: " << ownerName
                  << " | Balance: " << std::fixed << std::setprecision(2) << balance << "\n";
    }
};

class SavingsAccount : public BankAccount
{
private:
    double interestRate;

public:
    SavingsAccount(std::string owner, int accNum, double initialBalance, double rate)
        : BankAccount(std::move(owner), accNum, initialBalance), interestRate(rate)
    {
    }

    void applyInterest() override
    {
        double interest = balance * (interestRate / 100.0);
        balance += interest;
        std::cout << "Applied interest (" << interestRate << "%): +"
                  << std::fixed << std::setprecision(2) << interest << "\n";
    }

    void displaySummary() const override
    {
        std::cout << "Savings #" << accountNumber
                  << " | Owner: " << ownerName
                  << " | Balance: " << std::fixed << std::setprecision(2) << balance
                  << " | Rate: " << interestRate << "%\n";
    }
};

int main()
{
    std::string name;
    int accNum;
    double initialBalance, rate;

    std::cout << "--- C++20 OOP Bank Account Lab ---\n";
    std::cout << "Enter account holder name (single word): ";
    if (!(std::cin >> name))
    {
        return 1;
    }
    std::cout << "Enter account number: ";
    if (!(std::cin >> accNum))
    {
        return 1;
    }
    std::cout << "Enter opening balance: ";
    if (!(std::cin >> initialBalance))
    {
        return 1;
    }
    std::cout << "Enter savings interest rate (%): ";
    if (!(std::cin >> rate))
    {
        return 1;
    }

    std::unique_ptr<BankAccount> account =
        std::make_unique<SavingsAccount>(name, accNum, initialBalance, rate);

    int choice;
    while (true)
    {
        std::cout << "\n1. Deposit\n2. Withdraw\n3. Apply Interest\n4. Summary\n5. Exit\n";
        std::cout << "Choice: ";
        if (!(std::cin >> choice))
        {
            break;
        }

        if (choice == 1)
        {
            double amt;
            std::cout << "Amount to deposit: ";
            if (std::cin >> amt)
            {
                account->deposit(amt);
            }
        }
        else if (choice == 2)
        {
            double amt;
            std::cout << "Amount to withdraw: ";
            if (std::cin >> amt)
            {
                account->withdraw(amt);
            }
        }
        else if (choice == 3)
        {
            account->applyInterest();
        }
        else if (choice == 4)
        {
            account->displaySummary();
        }
        else if (choice == 5)
        {
            std::cout << "Exiting.\n";
            return 0;
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
