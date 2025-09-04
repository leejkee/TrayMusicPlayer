//
// Created by 31305 on 2025/8/26.
//
#pragma once
#include <memory>
#include <QObject>

namespace Tray::App
{
class CoreServicePrivate;
class CoreService final: public QObject
{
public:
    explicit CoreService(QObject *parent = nullptr);

    /*!
     * @brief Initializes signal/slot connections between core components.
     */
    void initConnections();

    /*!
     * @brief Performs initial loading of settings and playlists.
     */
    void initPreload();

    ~CoreService() override;


Q_SIGNALS:

    /*!
     * @brief Emitted when handling a UI request to display the contents of a specific playlist.
     *        Sends the list of song titles for the given playlist.
     * @param key The identifier (key) of the playlist.
     * @param list A string list containing only the titles of the songs in the playlist.
     */
    void signalSendUiCurrentTitleList(const QString& key, const QStringList& list);


    /*!
     * @brief Emitted when the current music source changes, to notify the UI
     *        about the new playback state. Used to update the highlighted item,
     *        display the song title, and set duration for progress tracking.
     * @param index The index of the currently playing song within its playlist.
     * @param listKey The identifier (key) of the current playlist.
     * @param duration The total duration of the current song in milliseconds.
     */
    void signalNotifyUiCurrentMusicChanged(int index
                                          , const QString& listKey
                                          , int duration);

    /*!
     * @brief Emitted when the playback status changes, to notify the UI about play/pause state.
     * @param b True if the player is now playing, false if paused.
     */
    void signalNotifyUiPlayingStatusChanged(bool b);

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
     * @brief Moves or copies a song between playlists.
     * @param sourceListKey The identifier of the source playlist.
     * @param destinationListKey The identifier of the destination playlist.
     * @param index The index of the song in the source playlist to move/copy.
     */
    void addMusicToList(const QString& sourceListKey
                        , const QString& destinationListKey
                        , int index);

    /*!
     * @brief Removes a song from a specified playlist.
     * @param key The identifier of the playlist.
     * @param songTitle The title of the song to remove.
     */
    void removeMusicFromList(const QString& key, const QString& songTitle);
    /* Playlist Controller End */

    /* Settings Interface Begin */
    /*!
     * @brief Retrieves the list of all user playlist identifiers.
     * @return A list of playlist keys created by the user.
     */
    QStringList getUserListKeys();

    /*!
     * @brief Gets the list of directories monitored for local music.
     * @return A list of paths where the application scans for music files.
     */
    QStringList getLocalMusicPaths();

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
    /* Settings Interface End */

private:
    std::unique_ptr<CoreServicePrivate> d;
};

}
