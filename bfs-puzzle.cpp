#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <chrono>
#include <unordered_set>
#include <map>
#include <stack>

using namespace std;
class Node
{
public:
  vector<Node *> children;
  vector<int> initialVector;
  Node *parent;

  Node(vector<int> _initialVector, Node *_parent)
  {
    initialVector = _initialVector;
    parent = _parent;
  }

  void printPuzzle(vector<int> vectorValues)
  {
    int count = 0;
    for (auto i : vectorValues)
    {
      if (count % 3 == 0)
        cout << "\n";

      cout << i << ' ';
      count++;
    }
  }

  int findZero()
  {
    // Find index of zero in vector
    // find: Retorna ponteiro para a posição do zero
    // distance: retorna a posição do vetor que está o blank space
    vector<int>::iterator it = find(this->initialVector.begin(), this->initialVector.end(), 0);
    return static_cast<int>(distance(this->initialVector.begin(), it));
  }

  void moveUp()
  {
    int zPos = this->findZero();
    vector<int> temp = this->initialVector;

    if (zPos >= 3)
    {
      swap(temp[zPos], temp[zPos - 3]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }

  void moveDown()
  {
    int zPos = this->findZero();
    vector<int> temp = this->initialVector;

    if (zPos < 6)
    {
      swap(temp[zPos], temp[zPos + 3]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }

  void moveRight()
  {
    int zPos = this->findZero();
    vector<int> temp = this->initialVector;

    if (zPos % 3 < 2)
    {
      swap(temp[zPos], temp[zPos + 1]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }

  void moveLeft()
  {
    int zPos = this->findZero();
    vector<int> temp = this->initialVector;

    if (zPos % 3 > 0)
    {
      swap(temp[zPos], temp[zPos - 1]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }
};

int traceSolution(vector<Node *> sol, Node *g)
{
  Node *curr = g;
  sol.push_back(g);

  while (curr->parent != nullptr)
  {
    curr = curr->parent;
    sol.push_back(curr);
  }

  reverse(sol.begin(), sol.end());

  int depth = 0;
  for (auto i : sol)
  {
    depth += 1;
    // i->printPuzzle(i->initialVector);
    // cout << "\n";
  }
  return depth;
  // cout << "Depth: " << depth - 1 << endl;
}

class Puzzle
{
private:
  queue<Node *> queue;            // para open
  map<vector<int>, bool> visited; // para closed
  vector<int> finalVector;

public:
  int depth;
  int nodesCount;
  Puzzle(vector<int> _initialVector, vector<int> _finalVector)
  {
    Node *initialNode = new Node(_initialVector, nullptr);
    this->queue.push(initialNode);

    this->finalVector = _finalVector;
    this->nodesCount = 0;

    this->visited[initialNode->initialVector] = true;
  }

  void BFS(vector<int>)
  {
    vector<Node *> solution;

    cout << "Searching for solution..." << endl;

    while (!queue.empty())
    {
      Node *currentNode = this->queue.front();

      this->queue.pop();

      currentNode->moveUp();
      currentNode->moveLeft();
      currentNode->moveRight();
      currentNode->moveDown();

      this->nodesCount++;

      for (auto currentChild : currentNode->children)
      {

        // If currentChild is equal to finalVector retunr void
        if (currentChild->initialVector == this->finalVector)
        {

          // Print solution steps
          this->depth = traceSolution(solution, currentChild);
          // cout << "Nodes count: " << this->nodesCount << endl;
          return;
        }

        // Verificando se o nodo atual já foi visitado
        if (visited.count(currentChild->initialVector) == 0)
        {

          this->visited[currentChild->initialVector] = true;
          this->queue.push(currentChild);
        }
      }
    }
  }
};

int manhattan_distance_matrix(int puzzle[][3])
{
  int sum = 0;
  int n = 3;

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      int num = puzzle[i][j];
      if (num == 0)
      {
        continue; // Ignora o espaço vazio.
      }

      // Encontra a posição correta do número.
      int row_goal = num / n;
      int col_goal = num % n;

      // Calcula a distância de Manhattan.
      int row_diff = abs(i - row_goal);
      int col_diff = abs(j - col_goal);
      sum += row_diff + col_diff;
    }
  }

  return sum;
}

int main()
{

  vector<int> initialVector = {2, 4, 7, 0, 3, 6, 8, 1, 5};
  vector<int> finalVector = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  Puzzle puzzle(initialVector, finalVector);

  auto start_time = chrono::high_resolution_clock::now();

  puzzle.BFS(initialVector);
  int puzzleDistance[3][3] = {

      {
          0,
          3,
          6,
      },
      {8, 1, 5}};
  int distanceManhattan = manhattan_distance_matrix(puzzleDistance);
  auto end_time = chrono::high_resolution_clock::now();
  auto duration_sec = chrono::duration<double>(end_time - start_time).count();
  cout << puzzle.nodesCount << "," << puzzle.depth - 1 << "," << duration_sec << "," << 0 << "," << distanceManhattan << endl;

  return 0;
}
