#ifndef COST_BASE_NAVIGATOR
#define COST_BASE_NAVIGATOR

#include "navigator.h"

class CostBaseNavigator : public Navigator
{
    public:
    DijkstraNode navigate(int, int);

    private:
    int minNode(DijkstraNode*, bool*);
    void updateNode(int&, int&, DijkstraNode, DijkstraNode&);
};

#endif