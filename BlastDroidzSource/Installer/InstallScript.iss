; TCR compliant Sample install script.
; Basically put this file in the root for what you want included in the installer,
;   then put everything into the subdirectories as listed in the installer below.
;   Thanks to Dan Weiss (dweiss@digipen.edu) for the original version.
;
; !!NOTE!!
; There are three steps that you must perform in order to customize this script
;   for your project (see all "TODO" comments below):
;   1) Generate a new GUID for your game (the AppId).
;   2) Search-and-Replace "TeamA001" with your game name.
;   3) Update the redistributables required for your game.

[Setup]
; !!NOTE!!: The value of AppId uniquely identifies this application.  
; Do not use the same AppId value in installers for other applications.
; Failure to change this GUID will result in a penalty to the final project grade.
; TODO: (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{51973483-81F1-4B57-966F-64B8E4EC2213}

; Standard app data stuff
; TODO: Search-and-Replace "TeamA001" with your game name
AppName=BlastDroids
AppVerName=BlastDroids 1.0
AppPublisher=DigiPen Institute of Technology
AppPublisherURL=http://www.digipen.edu/
AppSupportURL=http://www.digipen.edu/

; Default path to the file storage directory.
; {pf} is the default program files directory set by Windows
DefaultDirName={pf}\DigiPen\TeamA001

; Start menu directory
DefaultGroupName=DigiPen\TeamA001

; Output directory for the installer.
OutputDir=.\INSTALLER

; Setup executable installer
OutputBaseFilename=BlastDroids_Setup

; Path to the DigiPen EULA (Needed to pass TCRs)
LicenseFile=INSTALLERFILES\DigiPen_EULA.txt

; Compression scheme for the installer. Check Inno Setup help files for more options.
Compression=lzma
SolidCompression=yes

; Path to the icon for the installer (TCR check requires custom icon)
SetupIconFile=.\INSTALLERFILES\SetupIcon.ico

; This allows the installer to run without admin privileges, which means you can't install
; in the Program Files, or change the registry. This is done to allow installation on Sandbox
; or other intermediate directory
PrivilegesRequired=none

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
; Creates an installer option to allow/disallow desktop shortcut
; Checked by default
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"


; This is the list of files that the installer should grab and install.
;
; Destination Directory Notes
;   {app} is the root directory that you will be installing to.
;   {temp} is a temporary directory that Windows sets that gets deleted after the
;      installer quits.
;   {userdocs} is the directory for My Documents/Documents on Windows XP, Vista, and 7.
;
; For more information on default paths to install to, check the "Constants" article
;   in the Inno Setup 5 documentation.
;
; I recommend placing any installers for required stuff (DirectX, PhysX, etc)
;   in the general structure below to keep things neat for you.
[Files]
; The game directoy is exactly what you want your install directory in program files to look like
Source: .\GAMEDIRECTORY\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs

; Include the redistributable programs and install them to the temp directory
; TODO: Update this list with the correct redistributables for your game.
;       If your game uses DirectX, then please refer to the following forum posting:
;	        https://distance.digipen.edu/mod/forum/discuss.php?d=19674&parent=39276
; NOTE: Projects built using Visual Studio require the Visual C++ Redistributable Package.
;   Download the Visual C++ Redistributable for your version of Visual Studio from
;   the Microsoft Downloads site.
Source: .\REDIST\*; DestDir: {tmp}; Flags: ignoreversion

; This is the list of shortcuts that the installer will setup for you.
; Of note, this will create the uninstaller automatically.
;
; Directory Notes
;   {group} is the start menu location that the game will install shortcuts to.
;   {commondesktop} is your Windows desktop directory.

[UninstallDelete]
Type: filesandordirs; Name: "{app}\Logs"


[Icons]
Name: {group}\BlastDroids; Filename: {app}\BlastDroids.exe; WorkingDir: {app}; IconFilename: "{app}\Icon.ico"
Name: {group}\{cm:UninstallProgram,TeamA001}; Filename: {uninstallexe}
Name: {commondesktop}\BlastDroids; Filename: {app}\BlastDroids.exe; Tasks: desktopicon; WorkingDir: {app}; IconFilename: "{app}\Icon.ico"


; List of items to execute in the installer.
; Note that this will run all executables in their silent versions as required by the TCRs.
;
; The last item being run is the installer option to automatically launch the game after
;   the installer exits as required by the TCRs.
; TODO: Update this list with the correct redistributables for your game.
[Run]
Filename: {tmp}\vc_redist.x86.exe; Parameters: /q; StatusMsg: Installing Visual C++ 2015 Redistributable...
; Filename: {tmp}\dxsetup.exe; Parameters: /q; StatusMsg: Installing DirectX...
Filename: {app}\BlastDroids.exe; Description: {cm:LaunchProgram,TeamA001}; Flags: nowait postinstall runascurrentuser skipifsilent
