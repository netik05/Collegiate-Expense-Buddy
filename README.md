Collegiate Expense-Buddy/CashFlow: A C++ Splitwise Solution

This is a C++ program for managing expenses and transfers within a group. It allows users to add expenses, make transfers, settle debts, and generate reports.

1)Greedy Algorithm

The algorithm used here to settle debts within a group is based on the concept of "minimum cash flow" or "minimum transaction" algorithm. This algorithm aims to minimize the total number of transactions required to settle all debts among group members.

At each step, it greedily selects the pair of people (one owing money and one being owed) with the maximum difference in their balances and settles the smallest of the two amounts between them. This process is repeated until all balances are close to zero.

Complexity - The time complexity of the algorithm used is O(N^2), where N is the number of people in the group. This is because for each person, we need to calculate their net balance by iterating over all transactions involving that person.

2)Kruskal's algorithm

The program uses the Minimum Spanning Tree (MST) algorithm to find the optimal way to settle debts within a group.

The Minimum Spanning Tree (MST) algorithm used in the program is Kruskal's algorithm.

The time complexity of the debt settlement algorithm in second program is (O(n^2 (log n)), where n is the number of people in the group.
