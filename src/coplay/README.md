<p align="center"> <img width="500" src="https://coaxion.games/res/coplayLogo.svg"/> </p>

Logo made by FLARE145.

# Coplay
A MPL2 licensed Source Engine Multiplayer SDK addon that enables the use of the Steam Datagram Relay for P2P connections with support for Linux and Windows.

If your mod implements Coplay please redirrect issue reports related to it [here](https://github.com/CoaXioN-Games/coplay/issues/new/choose).

## Usage
Mods implementing Coplay can make their own UI if they want to but there are commands and cvars that come standard.

| Command | Description | Usage |
| :-----: | :---------: | :---: |
| `coplay_about` | Reddirects to this Github page and prints the current version | `coplay_about` |
| `coplay_connect` | Connect to a server with a Ipv4 address, User SteamID64 or Lobby SteamID64 * | `coplay_connect (IP or SteamID64)` |
| `coplay_getconnectcommand` | Prints a command for others to use to connect to your server | `coplay_getconnectcommand` |
| `coplay_opensocket` | Allows your game to be joined via Coplay, this is automatically called on server creation if coplay_joinfilter is above -1 | `coplay_opensocket` |
| `coplay_closesocket` | Disables your game from being joined via Coplay, this will also kick currently connected players | `coplay_closesocket` |
| `coplay_listlobbies`* | List joinable lobbies | `coplay_listlobbies` |
| `coplay_invite`* | Brings up the game invite dialog if you're in a game | `coplay_invite` |


| Cvar | Description | Default value |
| :--: | :---------: | :-----------: |
| `coplay_joinfilter` | Sets who is allowed to join the game. -1: Off, 0: Invite Only, 1: Friends only, 2: Anyone(Lobby advertised if available*) | 1 |
| `coplay_timeoutduration` | How long in seconds to keep a connection around that has no game activity | 5 |
| `coplay_portrange_begin` ** | Where to start looking for ports to bind on | 3600 |
| `coplay_portrange_end` ** | Where to stop looking for ports to bind on | 3700 |
| `coplay_connectionthread_hz` | Number of times to service connections per second, it's unlikely you'll need to change this | 300 |
| `coplay_forceloopback` | Uses the loopback interface instead of others, only change this if you have issues | 1 |

\* : Requires Steam Lobbies, Lobbies are only available for mods with an appid on Steam.
\** :  Only change this if issues arise, a range of at least 64 is recommended.

# Adding to your mod

## Prerequistes

### Updating the Steamworks SDK
A more updated Steamworks SDK than the one that comes with the Source SDK is required to use Coplay, If you haven't already updated your mod's version you can find the latest download that works with [here](https://partner.steamgames.com/downloads/steamworks_sdk_160.zip).

If your mod still has references to `CSteamAPIContext`/`g_SteamAPIContext`/`steamapicontext` either replace them with the new global accessor equivelent (something like `steamapicontext->SteamFriends()->...` turns to `SteamFriends()->...`) or use the last version of the Steamworks SDK that still provides it, 159, found [here](https://partner.steamgames.com/downloads/steamworks_sdk_159.zip).

Steamworks versions above 157 do not apear to work with the Source SDK by default, if neither of the above versions work for you the download for version 157 is [here](https://partner.steamgames.com/downloads/steamworks_sdk_157.zip). When a fix is confirmed to work this README will include it.

1. Delete the `public/steam` folder of your mod's source tree and replace it with `public/steam` folder inside the downloaded zip, you're safe to delete the contained `lib` folder if you want.

2. Replace the steam_api.lib and libsteam_api.so files found in `lib/public` and `lib/public/linux32` with the ones in the zip under the folder `redistributable_bin`. Make sure to copy the 32 bit versions.

3. Rerun your VPC script and build.


### Fix host_thread_mode
Coplay enables this convar on start. Due to an engine bug, locally hosted servers will sometimes run faster than intended. host_thread_mode 2 fixes this but causes inputs by the host player to be discarded because of a desync.
This can be side-stepped by changing this statement in player.cpp CBasePlayer::IsUserCmdDataValid() from
```
bool bValid = ( pCmd->tick_count >= nMinDelta && pCmd->tick_count < nMaxDelta ) &&
				  // Prevent clients from sending invalid view angles to try to get leaf server code to crash
				  ( pCmd->viewangles.IsValid() && IsEntityQAngleReasonable( pCmd->viewangles ) ) &&
				  // Movement ranges
				  ( IsFinite( pCmd->forwardmove ) && IsEntityCoordinateReasonable( pCmd->forwardmove ) ) &&
				  ( IsFinite( pCmd->sidemove ) && IsEntityCoordinateReasonable( pCmd->sidemove ) ) &&
				  ( IsFinite( pCmd->upmove ) && IsEntityCoordinateReasonable( pCmd->upmove ) );
```
to
```
// when using host_thread_mode 1 or 2 on a listen server the client and server tickcount get desynced resulting in none of the hosts usermsgs getting accepted
// this first || fixes that and doesnt seem to do anything else as far as i can tell; Tholp
    bool bValid = ((!engine->IsDedicatedServer() && entindex() == 1) || ( pCmd->tick_count >= nMinDelta && pCmd->tick_count < nMaxDelta )) &&
				  // Prevent clients from sending invalid view angles to try to get leaf server code to crash
				  ( pCmd->viewangles.IsValid() && IsEntityQAngleReasonable( pCmd->viewangles ) ) &&
				  // Movement ranges
				  ( IsFinite( pCmd->forwardmove ) && IsEntityCoordinateReasonable( pCmd->forwardmove ) ) &&
				  ( IsFinite( pCmd->sidemove ) && IsEntityCoordinateReasonable( pCmd->sidemove ) ) &&
				  ( IsFinite( pCmd->upmove ) && IsEntityCoordinateReasonable( pCmd->upmove ) );
```

## Adding Coplay

1. First Either clone Coplay as a git submodule, or download it as a zip then place it into the root of your mod's source( Where folders such as `devtools`, `game` and `public` are. )

2. Open your mod's client .vpc file, and add the line
`$Include "$SRCDIR\coplay\src\coplay.vpc"`
to it somewhere at the top, if your mod has an appid on Steam you'll probably want to append `COPLAY_USE_LOBBIES` to your `$PreprocessorDefinitions`( Don't add this preprocessor if your mod does not have an appid, otherwise Coplay will be effectively nonfunctional ).

3. Rerun your VPC script and build.

4. If you have linker issues when building on Linux delete the libSDL2.so found in your mod's `src/lib/public/linux32` folder and retry.

5. Add the SDL2_net.dll and libSDL2_net.so (if your mod supports Linux) found in coplay/lib to you mod's /bin folder.

# FAQ

## How?
Coplay is a network relay that maps ports on your local machine to Steam datagram connections, this allows it to not require modifing engine code.

## Whats the difference if Steam's Lobby system is available?
Lobbies allow user hosted games to be advertised and joined akin to the server browser.

## Does custom content work?
Yes, as normal.

## My mod wont launch anymore! "Can't load library client"
Reading the instructions is recomended.

## I Can't move in my own local server!
Reading the instructions is recomended.

## Whats your favorite color?
Green, thanks for asking.
