#ifndef _SYNC_BOT_
#define _SYNC_BOT_

#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include "logging.h"
#include "Socket.h"
#include "Server.h"
#include "Client.h"
#include "FileUtils.h"
#include "SyncDir.h"
#include "SyncArgs.h"

using namespace std;

enum SyncBotState{
    SLEEP = 0,
    WAITING = 1,
    AUTH = 2,
    INFO_SHARE = 3,
    CHANGE_SHARE = 4,
    REALTIME_SYNC= 5
};

enum AuthState{
    NOT_STARTED = 0,
    WAITING_REMOTE_AUTH = 1,
    AUTHORIZED = 2
};

enum SyncAllowState{
    WAIT = 10,
    ALLOWED = 9,
    DENYED = 8
};
/*
* Message dialog:
*    Auth message:
*        auth [local password] [local sync dir] [scpPort]
*            scp port is optional
*    Login message:
*        login [host password]
*    Add directory to remote:
*        dir add [dir name];
*    Remove directory on remote:
*        dir rm [dir name];
*    Update/Add file to remote:
*        file up [file-name] [last-mod-time]
*    Remove file from remote:
*        file rm [file-name]
*    Signal to remote that all the info has been shared:
*        shared-all-info
*    Signal to remote that all the files has been shared:
*        shared-all-changes
*    Init micro-sync:
*        start-sync
*    Sync allowed:
*        allow-sync
*    Sync denyed:
*        deny-sync
*    End micro-sync:
*        end-sync
*    Make a directory:
*        mkdir [dir]
*    Delete a directory:
*        rm -Rf [dir]
*    Delete file:
*        rm -f [file]
*    
*/
/* Friendly bot that helps synchronizing files and dirs*/
class SyncBot{
public:
    //Start SyncBot using parsed command line args
    SyncBot(SyncArgs args);

    //start sync process, blocking
    bool run();
protected:
    //returns pointer to Client or Server
    static Socket* makeSocket(string ip, int port, bool server);
    static string getUserName();
    static list<string> splitStrInList(string s);

    //Tryes to get differences 
    //void lookForDiffs();
    void updateCycle();
    void update();

    void sleeping();
    void updateLocalDirIfNotBusy();

    void waiting();

    void authentication();
    bool someoneTryedToLogin();
    bool correctServerPassword();
    void setHostPasswdTry(string newTry);
    bool sendLoginMessage();
    bool sendAuthMessage();
    bool hasRemoteAuthorization();

    void infoShare();
    void sendChangeInfo(SyncChange change);
    void sendDirRemove(string dir);
    void sendDirAdd(string dir);
    void sendFileRemove(string file);
    void sendFileAdd(string file, time_t lastMod);
    void sendStartSync();
    void sendAllowSync();
    void sendDenySync();
    void sendEndSync();
    void sendSharedAllInfo();
    void printChangesToSend();

    void changeShare();
    void sendChange(SyncChange change);
    void sendDeleteFile(string file);
    void sendFile(string localFile, string remoteFile);
    void sendDeleteDir(string dir);
    void sendMkdir(string dir);
    void sendSharedAllChanges();

    void sync();
    
    /*//Sync all info
    void syncInfo();
    //Sync only dir info
    void syncDirInfo();
    //Sync only file info
    void syncFileInfo();
    //Send message to delete file/dir on remote too
    bool sendDelete(string path, bool dir = false);
    //Send message to update file/dir on remote too
    bool sendUpdate(string path, bool dir = false);*/

    /*//Calls scp to send changes
    void sendChangesToRemote();
    //Notifies remote that a update is on the way
    bool sendRequireUpdate();
    //Notifies the remote that it can start updating
    bool sendAllowUpdate();
    //Notifies remote that the update is finished
    bool sendFinishedUpdate();*/
    
    void treatMessagesCycle();
    void treatMessage(string message);
    void login(string password);
    void auth(string userPassword, string userName, string remoteDir, int transferPort);
    void dir(string op, string dir);
    void fileUp(string file, time_t lastMod);
    void fileRemove(string file);
    void remoteSharedAllInfo();
    void remoteSharedAllChanges();
    void remoteStartSync();
    void allowedToSync();
    void denyedToSync();
    void remoteEndSync();
    void mkdir(string message, string dir);
    void erase(string message, string obj);

    bool isServer;
    //receiving update from remote / sending update to remote
    //bool remoteUpdating, localUpdating;
    //mutex localUpdating, remoteUpdating;
    bool sharedAllInfoFlag, remoteSharedAllInfoFlag;
    bool sharedAllChangesFlag, remoteSharedAllChangesFlag;
    deque<SyncChange> onLocalButNotInRemote;

    mutex syncLock;
    //port for local socket
    int hostPort;
    string hostAddress;
    string hostPasswd;
    mutex loginMutex;
    string hostPasswdTry;

    int scpPort = -1;
    int remoteScpPort = -1;

    SyncDir localDir, remoteDir;

    string localDirName, remoteDirName;
    string localPasswd, remotePasswd;
    string localUserName, remoteUserName;
    string remoteAddress;
    Socket *socket = NULL;
    thread *updateThread = NULL;
    thread *treatMsgThread = NULL;

    SyncBotState state;
    AuthState authState;

    bool finishFlag;
    bool authByRemote;
    SyncAllowState syncAllowState;


};



#endif