setLocal EnableDelayedExpansion
@echo off

echo Time,File,Event,String > .\logs\allstrings.csv
echo Time,File,Event,Team,ID,X,Y,Rotation > .\logs\allspawn.csv
echo Time,File,Event,SourceName,SourceID,SourceTeam,SourceWeapon,DamageDealt,HitName,HitID,X,Y > .\logs\alldamage.csv
echo Time,File,Event,TimeSinceStart,TimeSinceSceneStart,DeltaTime,TimeScale > .\logs\alltime.csv
echo Time,File,Event,Team,ID,EntityName,X,Y,Rotation > .\logs\alltransform.csv
echo Time,File,Event,Team,ID,EntityName,X,Y > .\logs\alldeath.csv


for /F %%I IN ('dir /b /s *.zip') DO (
    "C:\Program Files\7-Zip\7z.exe" x -y -o".\logs\" "%%I" 
)

::@echo on
echo Files Unzipped
::@echo off

set /a x=0

for /r %%i IN (*.log) DO (
	FINDSTR L_string %%i > %%i.strings.csv
  FINDSTR L_spawn %%i > %%i.spawn.csv
  FINDSTR L_damage %%i > %%i.damage.csv
  FINDSTR L_time %%i > %%i.time.csv
  FINDSTR L_transform %%i > %%i.transform.csv
  FINDSTR L_death %%i > %%i.death.csv
	FINDSTR L_State %%i > %%i.statePushPop.csv
	
	FINDSTR L_string %%i > logs\allstrings.csv
  FINDSTR L_spawn %%i >> logs\allspawn.csv
  FINDSTR L_damage %%i >> logs\alldamage.csv
  FINDSTR L_time %%i >> logs\alltime.csv
  FINDSTR L_transform %%i >> logs\alltransform.csv
  FINDSTR L_death %%i >> logs\alldeath.csv
	
  echo. >> logs\allstrings.csv
  echo. >> logs\allspawn.csv
  echo. >> logs\alldamage.csv
  echo. >> logs\alltime.csv
  echo. >> logs\alltransform.csv
  echo. >> logs\alldeath.csv
)

::@echo on
echo Files Parsed
::@echo off

::for /r %%i IN (*.spawn) DO (
::	for /f "tokens=* delims= " %%a in (%%i) do (
::		echo %%a,EXCELCOMMANDSHERE >> %%i.csv
::	)
::	rm %%i
::)
::
::@echo on
::echo SpawnDone
::@echo off
::
::for /r %%i IN (*.damage) DO (
::	for /f "tokens=* delims= " %%a in (%%i) do (
::		echo %%a,EXCELCOMMANDSHERE >> %%i.csv
::	)
::	rm %%i
::)
::
::@echo on
::echo DamageDone
::@echo off
::
::for /r %%i IN (*.time) DO (
::	for /f "tokens=* delims= " %%a in (%%i) do (
::		echo %%a,EXCELCOMMANDSHERE >> %%i.csv
::	)
::	rm %%i
::)
::
::@echo on
::echo TimeDone
::@echo off
::
::for /r %%i IN (*.transform) DO (
::	for /f "tokens=* delims= " %%a in (%%i) do (
::		echo %%a,EXCELCOMMANDSHERE >> %%i.csv
::	)
::	rm %%i
::)
::
::@echo on
::echo TransformDone
::@echo off
::
::for /r %%i IN (*.death) DO (
::	for /f "tokens=* delims= " %%a in (%%i) do (
::		echo %%a,EXCELCOMMANDSHERE >> %%i.csv
::	)
::	rm %%i
::)
::
::@echo on
::echo DeathDone
::@echo off
