#ifndef COST_BASE_NAVIGATOR
#define COST_BASE_NAVIGATOR

#include <vector>
#include "navigator.h"

class CostBaseNavigator : public Navigator
{
    public:
    DijkstraNode navigate(int, int, Clock);

    private:
    int minNode(std::vector<DijkstraNode>*, bool*);
    void updateNode(int&, int&, std::vector<DijkstraNode>, std::vector<DijkstraNode>&, Clock);
};

#endif