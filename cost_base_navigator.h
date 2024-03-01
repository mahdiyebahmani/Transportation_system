#ifndef COST_BASE_NAVIGATOR
#define COST_BASE_NAVIGATOR

#include <iostream>
#include <climits>
#include <map>
#include <vector>
#include <algorithm>
#include "path.hpp"
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