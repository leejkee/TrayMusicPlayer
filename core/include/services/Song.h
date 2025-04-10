#pragma once
#include <QString>


namespace Core::Service {
    struct SongInfo {
        SongInfo(const QString &name, const int length, const unsigned index) : title(name), duration(length),
            index(index) {
        }

        QString title;
        int duration;
        unsigned index;
    };

    class Song {
    public:
        Song(const Song &song) = default;

        explicit Song(const QString &songFilePath);

        [[nodiscard]] int getDuration() const {
            return m_duration;
        }

        [[nodiscard]] QString getName() const {
            return m_Name;
        }

        [[nodiscard]] QString getPath() const {
            return m_path;
        }

        [[nodiscard]] QString getArtist() const {
            return m_Artist;
        }

        [[nodiscard]] QString getTitle() const {
            return m_title;
        }

        [[nodiscard]] SongInfo toSongInfo() const;
        static QString removeSuffix(const QString &str);

    private:
        QString m_Name;
        QString m_Artist;
        QString m_title;
        QString m_path;
        int m_duration{0};

        ///
        /// @param path should be the @m_path
        /// @return the length of this music(Format: 00:00)
        static int musicLength(const QString &path);

        static QString convertToArtist(const QString &str);

        static QString convertToName(const QString &str);

        void songInitByTagLib(const QString &path);
    };
}
