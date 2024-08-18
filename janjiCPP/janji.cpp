#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class Member {
private:
  std::string name;
  int totalContribution;
  int savings;
  int debt;

public:
  Member(std::string _name, int _totalContribution)
      : name(_name), totalContribution(_totalContribution), savings(0), debt(0) {}

  std::string getName() const {
    return name;
  }

  void addToTotalContribution(int amount) {
    totalContribution += amount;
  }

  void addToTotalDebt(int amount) {
    debt += amount;
  }

  void addStaticDebt(float interest) {
    debt = static_cast<int>(debt * (1 + interest));
  }

  int getTotalContribution() const {
    return totalContribution;
  }

  int getSavings() const {
    return savings;
  }

  int getDebt() const {
    return debt;
  }

  void save(int amount) {
    savings += amount;
  }

  void reduceDebt(int payment) {
    debt -= payment;
    if (debt < 0) {
      debt = 0; // Ensure debt doesn't go negative
      savings += std::abs(debt); // Add any excess payment as savings
    }
  }

  void displayDetails() const {
    std::cout << "Member: " << name << " - Total Contribution: " << totalContribution
              << " - Savings: " << savings << " - Debt: " << debt << std::endl;
  }
};

class Contribute {
private:
  Member& member; // Reference to the member object
  int amount;

public:
  Contribute(Member& _member, int _amount) : member(_member), amount(_amount) {}

  void performContribution() {
    member.addToTotalContribution(amount);
  }
};

class Payment {
private:
  Member& member; // Reference to the member object
  int amount;

public:
  Payment(Member& _member, int _amount) : member(_member), amount(_amount) {}

  void performPayment() {
    member.reduceDebt(amount);
  }
};

class Borrow {
private:
  Member& member; // Reference to the member object
  int amount;
  float interestRate;

public:
  Borrow(Member& _member, int _amount, float _interestRate)
      : member(_member), amount(_amount), interestRate(_interestRate) {}

  void borrowMoney() {
    member.addToTotalDebt(amount);
    member.addStaticDebt(interestRate);
  }
};

// Functions to manage members and interact with Contribute, Payment, and Borrow classes

void addMember(std::vector<Member>& members, const std::string& name, int totalContribution) {
  members.push_back(Member(name, totalContribution));
}

void displayMembers(const std::vector<Member>& members) {
  for (const auto& member : members) {
    member.displayDetails();
  }
}

void determineBeneficiaryOrder(std::vector<Member>& members) {
  std::sort(members.begin(), members.end(), [](const Member& a, const Member& b) {
    if (a.getTotalContribution() == b.getTotalContribution()) {
      return a.getName() < b.getName(); // Sort alphabetically if contributions are equal
    }
    return a.getTotalContribution() > b.getTotalContribution();
  });

  std::cout << "Beneficiary Order:" << std::endl;
  for (const auto& member : members) {
    std::cout << member.getName() << std::endl;
  }
}

void fineForLatePayment(std::vector<Member>& members, int amount, float interestRate) {
  for (auto& member : members) {
    Borrow borrow(member, amount, interestRate);
    borrow.borrowMoney();
  }
}

void socialContribution(std::vector<Member>& members, const std::string& memberName, int amount) {
  for (auto& member : members) {
    if (member.getName() == memberName) {
      Contribute contribution(member, amount);
      contribution.performContribution();
      return;
    }
  }
}
void exitJangi(std::vector<Member>& members, std::string& exitMemberName) {
  // Find the member by name
  for (auto it = members.begin(); it != members.end(); ++it) {
    if (it->getName() == exitMemberName) {
      // Check if there's outstanding debt
      if (it->getDebt() == 0) {
        members.erase(it); // Remove member if no debt
        std::cout << exitMemberName << " has exited Jangi successfully." << std::endl;
        break;
      } else {
        std::cout << exitMemberName << " cannot exit Jangi with outstanding debt." << std::endl;
      }
      return; // Exit the function after processing the found member
    }
  }

  // If member not found, inform the user
  std::cout << exitMemberName << " not found in Jangi." << std::endl;
}



int main() {
  std::vector<Member> members;
  float interestRate = 0.05; // Interest rate for debt

  std::string choice;
  while (true) {
    std::cout << "\n1. Add a member\n2. Display members\n3. Determine beneficiary order\n4. Fine for late payment\n5. Social contribution\n6. Make a payment\n7. Borrow money\n8. Exit from Jangi\n9. Exit program" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (std::stoi(choice)) {
      case 1: {
        std::string name;
        int totalContribution;
        std::cout << "Enter member's name: ";
        std::cin >> name;
        std::cout << "Enter member's total contribution: ";
        std::cin >> totalContribution;
        addMember(members, name, totalContribution);
        break;
      }
      case 2:
        displayMembers(members);
        break;
      case 3:
        determineBeneficiaryOrder(members);
        break;
      case 4: {
        int latePaymentAmount;
        std::cout << "Enter late payment amount: ";
        std::cin >> latePaymentAmount;
        fineForLatePayment(members, latePaymentAmount, interestRate);
        break;
      }
      case 5: {
        std::string memberName;
        int socialContributionAmount;
        std::cout << "Enter member's name for social contribution: ";
        std::cin >> memberName;
        std::cout << "Enter social contribution amount: ";
        std::cin >> socialContributionAmount;
        socialContribution(members, memberName, socialContributionAmount);
        break;
      }
      case 6: {
        std::string memberName;
        int paymentAmount;
        std::cout << "Enter member's name for payment: ";
        std::cin >> memberName;
        std::cout << "Enter payment amount: ";
        std::cin >> paymentAmount;
        for (auto& member : members) {
          if (member.getName() == memberName) {
            Payment payment(member, paymentAmount);
            payment.performPayment();
            break;
          }
        }
        std::cout << "Member not found." << std::endl;
        break;
      }
      case 7: {
        std::string memberName;
        int borrowAmount;
        float borrowInterestRate;
        std::cout << "Enter member's name to borrow money: ";
        std::cin >> memberName;
        std::cout << "Enter borrow amount: ";
        std::cin >> borrowAmount;
        std::cout << "Enter borrow interest rate: ";
        std::cin >> borrowInterestRate;
        for (auto& member : members) {
          if (member.getName() == memberName) {
            Borrow borrow(member, borrowAmount, borrowInterestRate);
            borrow.borrowMoney();
            break;
          }
        }
        std::cout << "Member not found." << std::endl;
        break;
      }
      case 8:{
        std::string exitMemberName;
        std::cout << "Enter member's name to exit from Jangi: ";
        std::cin >> exitMemberName;
        exitJangi(members, exitMemberName);
        break;}
      case 9:{
        std::cout << "Exiting program..." << std::endl;
        return 0;
        }
      default:
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
  }

  return 0;
}