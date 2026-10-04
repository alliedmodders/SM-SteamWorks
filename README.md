SteamWorks
==========

Exposing SteamWorks functions to SourcePawn.

Compatibility
-------------

The extension is compiled against Steamworks SDK 1.32. The newer
`SteamGameServer014` interface used by `SteamWorks_SetAdvertiseServerActive` is
looked up manually, so the build does not require the newest SDK headers. If the
extension fails to load because the game ships an older or incompatible
Steamworks redistributable, refresh the game's copy from Steamworks SDK Redist:

1. Stop the game server.
2. Download SteamCMD and run:

   ```text
   steamcmd +login anonymous +app_update 1007 +quit
   ```

   App 1007 is Steamworks SDK Redist. SteamCMD places it below its
   `steamapps/common/Steamworks SDK Redist/` directory.
3. Copy the matching platform and architecture files from that directory over
   the game's existing Steamworks files, preserving the directory layout. On
   Windows these are typically `steamclient.dll`/`steamclient64.dll`; on Linux
   they are typically `steamclient.so`. There may be other supporting files as
   well, such as a tier0_s, vstdlib_s, and/or steamwebrtc binary.
4. Restart the server and check the extension log again.
