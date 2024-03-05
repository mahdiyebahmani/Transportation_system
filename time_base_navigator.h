#ifndef TIME_BASE_NAVIGATOR
#define TIME_BASE_NAVIGATOR

#include "navigator.h"

class TimeBaseNavigator : public Navigator
{
    public:
    DijkstraNode navigate(int, int, Clock);

    private:
    int minNode(DijkstraNode*, bool*);
    void updateNode(int&, int&, DijkstraNode, DijkstraNode&, Clock, int*, bool*);
};

#endif