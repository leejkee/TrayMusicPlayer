//
// Created by 31305 on 2025/8/26.
//
#pragma once
#include <memory>
#include <QObject>

namespace Tray::Core
{
class CoreServicePrivate;

class CoreService final : public QObject
{
    Q_OBJECT

public:
    explicit CoreService(QObject* parent = nullptr);

    /*!
     * @brief Initializes signal/slot connections between core components.
     */
    void initConnections();

    ~CoreService() override;

    /* Settings Interface Begin */
    /*!
     * @brief Retrieves the list of all user playlist identifiers.
     * @return A list of playlist keys created by the user.
     */
    [[nodiscard]] QStringList getUserListKeys() const;

    /*!
     * @brief Gets the list of directories monitored for local music.
     * @return A list of paths where the application scans for music files.
     */
    [[nodiscard]] QStringList getLocalMusicPaths() const;

    [[nodiscard]] QString getPreloadKey() const;

    [[nodiscard]] QStringList getTitleList(const QString& listKey) const;

    [[nodiscard]] int getDefaultVolume() const;

    /* Settings Interface End */

Q_SIGNALS:
    /*!
     * @brief Emitted when handling a UI request to display the contents of a specific playlist.
     *        Sends the list of song titles for the given playlist.
     * @param key The identifier (key) of the playlist.
     * @param list A string list containing only the titles of the songs in the playlist.
     */
    void signalCurrentTitleListChanged(const QString& key
                                       , const QStringList& list);

    /*!
     * @brief Emitted when the current music source changes, to notify the UI
     *        about the new playback state. Used to update the highlighted item,
     *        display the song title, and set duration for progress tracking.
     * @param index The index of the currently playing song within its playlist.
     * @param listKey The identifier (key) of the current playlist.
     * @param duration The total duration of the current song in milliseconds.
     */
    void signalCurrentMusicSourceChanged(int index
                                         , const QString& listKey
                                         , int duration);

    /*!
     * @brief Emitted when the playback status changes, to notify the UI about play/pause state.
     * @param b True if the player is now playing, false if paused.
     */
    void signalPlayingStatusChanged(bool b);

    /*!
     * @brief Emitted when the playback position of the current track changes.
     * @param pos The current playback position in milliseconds.
     */
    void signalPlayerPositionChanged(qint64 pos);

    /*!
     * @brief Emitted when the playback mode changes.
     * @param mode The new playback mode (0: Sequential, 1: Playlist Repeat, 
     *             2: Single Repeat, 3: Random).
     */
    void signalPlayModeChanged(int mode);

    /*!
     * @brief Emitted when a new user playlist is created.
     * @param key The identifier of the newly created playlist.
     */
    void signalUserPlaylistAdded(const QString& key);

    /*!
     * @brief Emitted when a user playlist is deleted.
     * @param key The identifier of the removed playlist.
     */
    void signalUserPlaylistRemoved(const QString& key);

    /*!
     * @brief Emitted when the list of user playlist keys is updated.
     * @param list The updated list of all user playlist identifiers.
     */
    void signalUserKeysChanged(const QStringList& list);

    /*!
     * @brief Emitted when the contents of a playlist in the cache are modified.
     * @param key The identifier of the modified playlist.
     * @param list The updated list of song titles in the playlist.
     */
    void signalListCacheChanged(const QString& key, const QStringList& list);

    /*!
     * @brief Emitted when the list of local music scan directories changes.
     * @param list The updated list of directory paths for local music scanning.
     */
    void signalLocalPathsChanged(const QStringList& list);

    /*!
     * @brief Emitted when the current active playlist changes.
     * @param key The identifier of the newly active playlist.
     */
    void signalCurrentListChanged(const QString& key);

    void signalLyricChanged(const QString& musicTitle, const QStringList& list
                            , const QList<int64_t>& lrc_timing);

    void signalLyricLineIndexChanged(int index);

    void signalMuteChanged(bool b);

    void signalVolumeChanged(float volume);

public Q_SLOTS:
    /* Player Controller Begin */

    /*!
     * @brief Sets the audio volume of the player.
     * @param volume The volume level as a percentage (0-100).
     */
    void setVolume(int volume);

    /*!
     * @brief Toggles between play and pause states.
     */
    void playToggle();

    /*!
     * @brief Starts or resumes playback.
     */
    void play();

    /*!
     * @brief Pauses the current playback.
     */
    void pause();

    /*!
     * @brief Toggles between muted and unmuted states.
     */
    void muteToggle();

    /*!
     * @brief Mutes the audio output.
     */
    void muteOn();

    /*!
     * @brief Unmutes the audio output.
     */
    void muteOff();

    /*!
     * @brief Sets the current playback position.
     * @param position The position to seek to in milliseconds.
     */
    void setPlayerPosition(qint64 position);
    /* Player Controller End */


    /* Playlist Controller Begin */

    /*!
     * @brief Handles playlist item selection from UI and changes current playlist and song.
     * @param listKey The identifier of the playlist to switch to.
     * @param index The index of the song to play within the playlist.
     */
    void handlePlaylistItemSelection(const QString& listKey, int index);

    /*!
     * @brief Switches playback to the next song.
     */
    void nextMusic();

    /*!
     * @brief Switches playback to the previous song.
     */
    void preMusic();

    /*!
     * @brief Cycles through playback modes.
     * Cycles between: sequential, playlist repeat, single repeat, and random modes.
     */
    void changePlayMode();

    /*!
     * @brief Handles UI requests to display a specific playlist's contents.
     * @param listKey The identifier of the playlist to display.
     * Triggers a signal containing the playlist contents.
     */
    void handleDisplayPlaylist(const QString& listKey);

    /*!
     * @brief Creates a new empty user playlist.
     * @param listKey The identifier for the new playlist.
     * Creates an empty table in the database for the new playlist.
     */
    void addUserPlaylist(const QString& listKey);

    /*!
     * @brief Removes a user playlist.
     * @param listKey The identifier of the playlist to remove.
     * Deletes the playlist table and updates settings accordingly.
     */
    void removeUserPlaylist(const QString& listKey);

    /*!
     * @brief Copies a song between playlists.
     * @param sourceListKey The identifier of the source playlist.
     * @param destinationListKey The identifier of the destination playlist.
     * @param index The index of the song in the source playlist to move/copy.
     */
    void copyMusicToList(const QString& sourceListKey
                         , const QString& destinationListKey
                         , int index);

    /*!
     * @brief Removes a song from a specified playlist.
     * @param key The identifier of the playlist.
     * @param songTitle The title of the song to remove.
     */
    void removeMusicFromList(const QString& key, const QString& songTitle);
    /* Playlist Controller End */

    /*!
     * @brief Adds a new directory to scan for local music files.
     * @param path The directory path to add to the scan list.
     */
    void appendLocalMusicPath(const QString& path);

    /*!
     * @brief Removes a directory from the local music scan list.
     * @param path The directory path to remove from scanning.
     */
    void removeLocalMusicPath(const QString& path);

private:
    std::unique_ptr<CoreServicePrivate> d;
};
}
