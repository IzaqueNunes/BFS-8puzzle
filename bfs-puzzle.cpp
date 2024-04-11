#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <chrono>

class Node
{
public:
  std::vector<Node *> children;
  std::vector<int> initialVector;
  Node *parent;

  Node(std::vector<int> _initialVector, Node *_parent)
  {
    initialVector = _initialVector;
    parent = _parent;
  }

  void printPuzzle(std::vector<int> vectorValues)
  {
    int count = 0;
    for (auto i : vectorValues)
    {
      if (count % 3 == 0)
        std::cout << "\n";

      std::cout << i << ' ';
      count++;
    }
  }

  int findZero()
  {
    // Find index of zero in vector
    std::vector<int>::iterator it = std::find(this->initialVector.begin(), this->initialVector.end(), 0);
    return static_cast<int>(std::distance(this->initialVector.begin(), it));
  }

  void moveUp()
  {
    int zPos = this->findZero();
    std::vector<int> temp = this->initialVector;

    if (zPos >= 3)
    {
      std::swap(temp[zPos], temp[zPos - 3]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }

  void moveDown()
  {
    int zPos = this->findZero();
    std::vector<int> temp = this->initialVector;

    if (zPos < 6)
    {
      std::swap(temp[zPos], temp[zPos + 3]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }

  void moveRight()
  {
    int zPos = this->findZero();
    std::vector<int> temp = this->initialVector;

    if (zPos % 3 < 2)
    {
      std::swap(temp[zPos], temp[zPos + 1]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }

  void moveLeft()
  {
    int zPos = this->findZero();
    std::vector<int> temp = this->initialVector;

    if (zPos % 3 > 0)
    {
      std::swap(temp[zPos], temp[zPos - 1]);
      Node *child = new Node(temp, this);
      children.push_back(child);
    }
  }
};

bool isVisited(std::queue<Node *> queue, Node *currentNode)
{
  bool exist = false;

  while (!queue.empty())
  {
    if (queue.front()->initialVector == currentNode->initialVector)
    {
      exist = true;
    }

    queue.pop();
  }

  return exist;
}

int traceSolution(std::vector<Node *> sol, Node *g)
{
  Node *curr = g;
  sol.push_back(g);

  while (curr->parent != nullptr)
  {
    curr = curr->parent;
    sol.push_back(curr);
  }

  std::reverse(sol.begin(), sol.end());

  int depth = 0;
  for (auto i : sol)
  {
    depth += 1;
    // i->printPuzzle(i->initialVector);
    // std::cout << "\n";
  }
  return depth;
  // std::cout << "Depth: " << depth - 1 << std::endl;
}

class Puzzle
{
private:
  std::queue<Node *> queue;
  std::queue<Node *> visited;
  std::vector<int> finalVector;

public:
  int depth;
  int nodesCount;
  Puzzle(std::vector<int> _initialVector, std::vector<int> _finalVector)
  {
    Node *initialNode = new Node(_initialVector, nullptr);
    this->queue.push(initialNode);

    this->finalVector = _finalVector;
    this->nodesCount = 0;

    this->visited.push(initialNode);
  }

  void BFS(std::vector<int> initialVector)
  {
    std::vector<Node *> solution;

    std::cout << "Searching for solution..." << std::endl;

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
          // std::cout << "Solution found :)" << std::endl;

          // Print solution steps
          this->depth = traceSolution(solution, currentChild);
          // std::cout << "Nodes count: " << this->nodesCount << std::endl;
          return;
        }

        if (!isVisited(this->visited, currentChild))
        {

          this->visited.push(currentChild);
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

  // Representa a configuração final do quebra-cabeça.
  // int goal[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

  // Itera sobre as posições da matriz.
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
  std::vector<int> initialVector = {
      0, 6, 1,
      7, 4, 2,
      3, 8, 5};

  std::vector<int> finalVector = {
      0, 1, 2,
      3, 4, 5,
      6, 7, 8};

  Puzzle puzzle(initialVector, finalVector);

  auto start_time = std::chrono::high_resolution_clock::now();

  puzzle.BFS(initialVector);
  int puzzleDistance[3][3] = {{
                                  0,
                                  6,
                                  1,
                              },
                              {
                                  7,
                                  4,
                                  2,
                              },
                              {3, 8, 5}};
  int distanceManhattan = manhattan_distance_matrix(puzzleDistance);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration_sec = std::chrono::duration<double>(end_time - start_time).count();
  std::cout << "Nodos expandidos: " << puzzle.nodesCount << '\n'
            << "Comprimento da Solucao otima: " << puzzle.depth - 1 << '\n'
            << "Valor medio da funcao heuristica: 0 \n"
            << "Heuristica no estado inicial: " << distanceManhattan << "\n";
  std::cout << "Tempo para solucao: " << duration_sec << " segundos" << std::endl;

  return 0;
}
