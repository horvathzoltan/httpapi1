#include "globals.h"
#include "zactionhelper.h"

zActionHelper::zActionHelper()
{

}

const zAction* zActionHelper::find(const QList<zAction>& actions, QString m, QString p)
{
    //for (auto i = actions.begin(); i != actions.end(); ++i)
    zforeach(i, actions)
    {
        if(i->method == m && i->path == p)
        {
            return i.operator->();
        }
    }
    return nullptr;
}

const zAction *zActionHelper::find(const QList<zAction>& actions, const zRequest& r)
{
    return find(actions, r.method, r.url.path());
}

bool zActionHelper::contains(const QList<zAction>& actions, QString m, QString p)
{
    return(find(actions, m, p)!=nullptr);
}
