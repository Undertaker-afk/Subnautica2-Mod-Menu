#pragma once

#include <string>
#include <vector>

namespace Features {
    namespace ESP {
        struct ResourceFilterEntry {
            std::string label;
            bool selected;
        };

        void Initialize();
        void Render();
        void Shutdown();
        void RefreshResourceFilters();
        const std::vector<ResourceFilterEntry>& GetResourceFilters();
        void SetResourceFilterSelected(std::size_t index, bool selected);
        void SetAllResourceFilters(bool selected);
        bool HasAnySelectedResourceFilter();
    }
}
