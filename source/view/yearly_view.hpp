#pragma once

#include "year_view_base.hpp"

#include "../model/date.hpp"
#include "calendar_component.hpp"
#include "highlight_menu.hpp"
#include "promptable.hpp"


#include <array>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <sstream>

namespace clog::view {

using namespace ftxui;

class YearView : public YearViewBase {
    InputHandlerBase* m_handler;
    ScreenInteractive m_screen;
    std::shared_ptr<Calendar> m_calendarButtons;
    std::shared_ptr<WindowedMenu> m_tagsMenu, m_sectionsMenu;
    std::shared_ptr<Promptable> m_rootComponent;
    Element m_logFileContentsPreview;

    const model::YearMap<bool>* m_highlightedLogsMap = nullptr;
    const model::YearMap<bool>* m_availabeLogsMap    = nullptr;
    std::vector<std::string> m_tags {}, m_sections {};

public:
    YearView(const model::Date& today);

    void run() override;
    void stop() override;

    void showCalendarForYear(unsigned year) override;

    void prompt(std::string message, std::function<void()> onYesCallback)  override{
        m_rootComponent->prompt(message, onYesCallback);
    }

    int& selectedTag()  override{ return m_tagsMenu->selected(); }
    int& selectedSection()  override{ return m_sectionsMenu->selected(); }
    
    void setInputHandler(InputHandlerBase* handler)  override{ m_handler = handler; }

    void setAvailableLogsMap(const model::YearMap<bool>* map)  override{ m_availabeLogsMap = map; }
    void setHighlightedLogsMap(const model::YearMap<bool>* map)  override{ m_highlightedLogsMap = map; }

    void setTagMenuItems(std::vector<std::string> items)  override{ m_tags = std::move(items); }

    void setSectionMenuItems(std::vector<std::string> items)  override{ m_sections = std::move(items); }

    void setPreviewString(const std::string& string) override;
    
    // temporay restore terminal to its roriginal state
    // to executa a function. used to start the editor
    void withRestoredIO(std::function<void()> func)  override{ m_screen.WithRestoredIO(func)(); }

    model::Date getFocusedDate() const  override{ return m_calendarButtons->getFocusedDate(); }

private:

    std::shared_ptr<Promptable> makeFullUIComponent();
    CalendarOption makeCalendarOptions(const Date& today);
};

}  // namespace clog::view
