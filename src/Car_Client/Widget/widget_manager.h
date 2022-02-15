#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H

#include "Widget/beginwidget.h"
#include "Widget/login.h"
#include "Widget/main_in.h"
#include "Widget/main_inmanage.h"
#include "Widget/main_interface.h"
#include "Widget/main_out.h"
#include "Widget/main_playback.h"
#include "Widget/main_replay.h"
#include "Widget/main_vehicl_equery.h"
#include "Widget/register.h"
#include "Widget/set.h"
#include <vector>

class Widget_Manager
{

public:
    Widget_Manager();
    static vector <Widget_Manager *> winArry;

signals:

public slots:
};

#endif // WIDGET_MANAGER_H
