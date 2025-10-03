#pragma once
#include <QObject>
#include <QStringList>
#include <memory>

namespace Tray::Core
{
class SettingsPrivate;

class Settings final : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject* parent = nullptr);

    explicit Settings(const QString& configFilePath, QObject* parent = nullptr);

    Settings(const Settings&) = delete;

    Settings& operator=(const Settings&) = delete;

    Settings(Settings&&) = delete;

    Settings& operator=(Settings&&) = delete;

    ~Settings() override;

    [[nodiscard]] QStringList getLocalMusicDirectories() const;

    [[nodiscard]] QStringList getKeysUserPlaylist() const;

    [[nodiscard]] int getDefaultVolume() const;

    [[nodiscard]] QString getPreloadKey() const;

Q_SIGNALS:
    void signalUserKeySetsChanged(const QStringList&);

    void signalLocalDirectoryChanged(const QStringList&);

public Q_SLOTS:
    void addLocalMusicDirectory(const QString& path);

    void addUserPlaylist(const QString& name);

    void removeUserPlaylist(const QString& name);

    void removeLocalMusicDirectory(const QString& path);

    void changeDefaultVolume(int volume);

    void changePreloadKey(const QString& key);

private:

    struct Key
    {
        static inline const auto PLAYLIST_KEY = QStringLiteral("Playlist");

        static inline const auto AUDIO_KEY = QStringLiteral("Audio");

        struct Playlist
        {
            static inline const auto MUSIC_PATHS = QStringLiteral("MusicPaths");

            static inline const auto USER_LISTS = QStringLiteral("UserLists");

            static inline const auto PRELOAD_LIST =
                    QStringLiteral("PreloadList");
        };

        struct Audio
        {
            static inline const auto DEFAULT_VOLUME =
                    QStringLiteral("DefaultVolume");
        };
    };

    struct SettingsData
    {
        struct Audio
        {
            int defaultVolume;
        };

        struct Playlist
        {
            QString preloadListKey;
            QStringList musicPaths;
            QStringList userLists;
        };

        // struct RememberSet
        // {
        //     QString theLastListKey;
        //     QString theLastMusic;
        // };

        Audio audio;
        Playlist playlist;
    };

    static inline const auto LOCAL_KEY = QStringLiteral("Local");

    static inline const SettingsData DEFAULT_SETTINGS{20, {LOCAL_KEY, {}, {}}};

    QString m_settingFilePath;

    SettingsData m_data;

    /// @brief Initializes the JSON settings file if it does not exist.
    ///
    /// This function checks for the existence of the settings file at the provided
    /// path. If the file is not found, it creates a new file with default settings
    /// and saves it. If the file already exists, no action is taken.
    ///
    /// @param filePath The absolute path to the settings file.
    /// @return `true` if the file exists or was successfully created; otherwise, `false`.
    [[nodiscard]] bool initJsonFile(const QString& filePath);

    /// @brief Parses the application's settings from a JSON file.
    ///
    /// This function reads the content of the JSON file specified by `m_path`,
    /// parses it into a QJsonDocument, and then populates the member variables
    /// of the SettingsData struct with the values from the JSON object.
    ///
    /// The function assumes the file at `m_path` exists and is accessible.
    /// If the file cannot be opened or the JSON is invalid, an error is logged
    /// and the function returns without modifying the current settings.
    void parseJson();

    [[nodiscard]] bool saveToJson() const;
};
}
