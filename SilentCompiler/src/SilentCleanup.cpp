#include "SilentCleanup.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef std::vector<SilentNode*> NodePtrList;

#define DEBUG 0

void Silent::SilentFreeNodes(std::vector<SilentNode*>* nodes)
{
    for(uint64 i = 0; i < nodes->size(); i++)
    {
        if((*nodes)[i] != NULL)
        {
#if DEBUG
            std::cout << "Freeing: " << (*nodes)[i]->name.data() << "\n";
#endif
            if((*nodes)[i]->type == SilentNodeType::structure)
            {
                NodePtrList* structVars = &((*nodes)[i]->structure->variables);
                for(uint64 j = 0; j < structVars->size(); j++)
                {
#if DEBUG
                    std::cout << "Freeing: " 
                    << (*nodes)[i]->name.data() 
                    << "." << (*structVars)[j]->name.data() 
                    << "\n";
#endif
                    free((*structVars)[j]->variable);
                }
            }
            free((*nodes)[i]->variable);
            free((*nodes)[i]);
        }
    }
}