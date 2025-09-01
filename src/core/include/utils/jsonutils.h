//
// Created by 31305 on 2025/9/1.
//
#pragma once
#include <QJsonObject>

namespace Tray::Core::Utils
{

template<typename T>
T getJsonValue(const QJsonObject& json, const QString& key, const T& defaultValue = T())
{
    if (json.contains(key))
    {
        const QJsonValue value = json.value(key);
        if constexpr (std::is_same_v<T, int>)
        {
            return value.toInt();
        }
        else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
        {
            return value.toDouble();
        }
        else if constexpr (std::is_same_v<T, QString>)
        {
            return value.toString();
        }
        else if constexpr (std::is_same_v<T, QStringList>)
        {
            if (value.isArray())
            {
                QStringList list;
                for (const auto& item : value.toArray())
                {
                    if (item.isString())
                    {
                        list.append(item.toString());
                    }
                }
                return list;
            }
        }

    }
    return defaultValue;
}

}

