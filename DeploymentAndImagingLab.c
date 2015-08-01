//1、Add a language,driver,package,or edition using a script
	
//	(1.Prepare the images for offline serviving
	
//	Step 1:Copy the base Windows image file
		copy "C:\Images\Win10_x64\sources\install.wim" C:\Images\WindowsWithOffice.wim
//	Step 2:Mount the Windows image file
		md C:\mount\windows
		Dism /Mount-Image /ImageFile:"C:\Images\WindowsWithOffice.wim" 
		     /Index:1 /MountDir:"C:\mount\windows" /Optimize
//	Step 3:Mount the Windows RE image file
		md C:\mount\winre
		Dism /Mount-Image /ImageFile:"C:\mount\windows\Windows\System32\Recovery\winre.wim" /index:1 /MountDir:"C:\mount\winre"
//	Step 4:Add drivers to Windows
//		<1.Add any.inf-style drivers needed for your hardware
		Dism /Add-Driver /Image:"C:\mount\windows" /Driver:"C:\Drivers\Pnp.Media.V1\media1.inf" /LogPath="C:\mount\dism.log"
		
//		<2.Verify that the driver is part of the image:
		Dism /Get-Drivers /Image:"C:\mount\windows"
		
//		<3.Remove any.inf drivers form previous images
		Dism /Remove-Driver /Image:"C:\mount\windows" /Driver:"VideoDriver-Old.inf"
//	Step 5:Add the files you need to modify the Start layout
		Note:In order to complete this step,you need to have the file LayoutModification.xml,created following the instructions in this Lab Pre-Requisites document.
//	Step 6:Add or change languages and Cortana features on demand
//		<1.Download the language pack(fr-fr\lp.cab) that matches the Windows image.Save this in this folder:C:\Languages\fr-fr x64\.

//		<2.Download the Features-On-Demand(FOD) packages that match the language and platform of the Windows image.Save this in this folder:C:\Languages\fr-fr x64\.

//		<3.Add languages,and Features On Demand to the Windows image.
		For packages with dependencies,make sure you install the packages in order.
		If not sure if the dependencies,it's OK to put them all in the same folder,and then add them all using the same DISM /Add-Package command.
		Dism /Add-Package /Image:"C:\mount\windows"
		     /PackagePath="C:\Languages\fr-fr x64\lp.cab"
		     /PackagePath="C:\Languages\fr-fr x64\Microsoft-Windows-LanguageFeatures-Basic-fr-fr-Package.cab"
		     ...
		
//		<4.Verify that the language package is part of the image
		Dism /Get-Packages /Image:"C:\mount\windows"

//		<5.Verify that the language components are part of the image
		Dism /Get-Capabilities /Image:"C:\mount\windows"
//	Step 7:Add Office Preview Uinversal apps
		Dism /Add-ProvisionedAppxPackage /Image:"C:\mount\windows" /PackagePath:"c:\samples\excelpreview\excel.appxbundle" /LicensePath:"c:\samples\excelpreview\excel_license.xml"
//	Step 8:Add Windows Universal apps
		Dism /Add-ProvisionedAppxPackage /Image:"C:\mount\windows" /PackagePath:"C:\samples\OneNote\OneNote.appxbundle" /LicensePath:"C:\sample\OneNote\OneNote_license.xml"
//	Step 9.Add Windows 8.1 Reader app
//		<1.
		Dism /Add-ProvisionedAppxPackage /Image:"C:\mount\windows" /PackagePath:"C:\downloads\reader\reader.appxbundle" /LicensePath:"C:\downloads\reader\reader_license.xml"

//		<2.Optional:Remove the base language(only needed for non-English regions)
		Dism /Remove-Packaage /Image:"C:\mount\windows" /PackageName:Microsoft-Windows-LanguageFeatures-Speech-en-us-Package~31bf3856ad364e35~amd64~~10.0.10120.0 /LogPath="C:\mount\dism.fod2.log"
		...

//		<3.Verify that the language package is no longer part of the image
		Dism /Get-Packages /Image:"C:\mount\windows"

//		<4.Verify that the language commponents are no longer part of the image
		Dism /Get-Capabilities /Image:"C:\mount\windows"

//		<5.Change the default language
		Dism /Set-AllIntl:fr-fr /Image:"C\mount\windows"
//	Step 10:Add languages to Windows RE
//		<1.
		Dism /add-package /image:"C\mount\winre" /packagepath:"C:\Program Files (x86)\Windows Kits\10\Assessment and Deployment Kit\Windows Preinstallation Environment\amd64\WinPE_OCs\fr-fr\lp.cab"
		...

//		<2.Verify that the language packages are part of the image
		Dism /Get-Packages /Image:"C:\mount\winre"

//		<3.Optional:Remove languages from Windows RE(only needed for non-English regions)
		Dism /Remove-Package /Image:"C:\mount\winre" /PackageName:WinPE-Rejuv-Package~31bf3856ad364e35~amd64~en-US~10.0.10120.0 /LogPath="C:\mount\dism.fod2.log"
		...

//		<4.Verify that the language packages are no longer part of the image
		Dism /Get-Packages /Image:"C:\mount\winre"
		
//		<5.Change the defualt language
		Dism /Set-AllIntl:fr-fr /Image:"C:\mount\winre"
//	Step 11:Upgrade the edition from Core to Pro
//		<1.Determine what images you can upgrade the image to
		Dism /Get-TargetEditions /Image:"C:\mount\windows"

//		<2.Upgrade the edition
		Dism /Set-Edition:Professional /Image:"C\mount\windows"
//	Step 12:Set the image to boot to audit mode
		mkdir C:\mount\windows\Windows\Panther
		copy C:\Sample\AnswerFiles\BootToAudit-x64.xml C:\mount\windows\Windows\Panther\unattend.xml
//	Step 13:Unmount images
//		<1.Close all applications that might access files from the image
//		<2.Commit the changes and unmount the Windows RE image
		Dism /Unmount-Image /MountDir:"C:\mount\winre" /Commit

//		<3.Make a backup copy of the updated Windows RE image
		xcopy C:\mount\windows\Windows\System32\Recovery\winre.wim C:\Images\winre_with_drivers_for_fabrikam_model_q.wim /ah

//		<4.Check the new size of the Windows RE image
		Dir "C:\mount\windows\Windows\System32\Recovery\winre.wim"
		rem == 3. Windows RE tools partition ===============
		create partition primary size=900

//		<5.Commit the changes and unmount the Windows image
		Dism /Unmount-Image /MountDir:"C:\mount\windows" /Commit
//	Step 14:Copy the image and deployment scripts to a USB key
//		<1.Plug in the Windows PE USB key and note the drive location
//		<2.Copy the image and the premade deployment scripts to a USB key
		copy C:\Images\WindowsWithOffice.wim D:
		copy C:\Samples\Scripts\* D:


/********************************************************************************************************************/
//2.Apply Windows images using a script
//	Step 1:Format and set up the hard drive partitions on the reference device
//		<1.Boot the reference device to Windows PE using the lab USB key or your own Windows PE USB key.
//		<2.Find the drive letter of the USB key by using diskpart:
		diskpart
		DISKPART> list volume
		DISKPART> exit

//		<3.Format the primary hard drive,create the partitions,and apply the image by using a pre-made lab script
		Walkthrough-Deploy-Compact.bat D:\Windowswith0.wim
		
//		<4.The scripts detect whether the PC is baseed on UEFI or BIOS,and then prompt you to press a key.

//		<5.The scripts apply the image to the drive,and then finish.
//	Step 2:Restart the device
	
/********************************************************************************************************************/
//3.Windows desktop apps and data:capturing changes
//	Step 1:Prepare a copy of ScanState
//		<1.On your technician PC,plug in another USB key or drive.

//		<2.
		mkdir D:\ScanState_x64
		copy "C:\Program Files (x86)\Windows Kits\10\Assessment and Deployment Kit\User State Migration Tool\amd64" D:\ScanState_x64
		copy "C:\Program Files (x86)\Windows Kits\10\Assessment and Deployment Kit\Windows Setup\amd64\Sources" D:\ScanState_x64
//	Step 2:Prepare a device for image capture
//		Get into audit mode
		*Boot up the reference device.
//		Verify drivers and packages
//		<1.Verify that the drivers appear correctly
		C:\Windows\System32\Dism /Get-Driver /Online
//		<2.Verify that the packages appear correctly
		C:\Windows\System32\Dism /Get-Driver /Online
//	Step 3:Install a Classic Windows application
//	Step 4:Capture your changes into a provisioning package
//		<1.Capture the changes into the provisioning package,and save it on the hard drive:
		D:\ScanState_x64\scanstate.exe /apps /ppkg C:\Recovery\Customizations\usmt.ppkg /o /c /v:13 /l:C:\ScanState.log
		Note:Optional:Delete the ScanState logfile: del C:\Scanstate.log

//		<2.Prepare the device for the end user
		C:\Windows\System32\Sysprep\sysprep /oobe /generalize /shutdown

//		Boot to Windows PE and prepare to capture
//		<1>.Plug in the lab USB key with Windows Preinstallation Environment(WinPE)
//		<2>.Turn on the device and press the key that opens the boot-device selection menu for the device

//		<3>.Optional:speed uo the optimization and image capture processes by setting the power scheme to High performance:
		powercfg /s 8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c

//		<4>.Find the drive letters by using DiskPart:
		diskpart
		DISKPART> list volume
		DISKPART> exit
//		Optimize the image to take up less drive space(optional)
//		<1>.Convert the Classic Windows applications and files outside of the provisioning package into pointer files which reference the contents inside the provisioning package
		Dism /Apply-CustomDataImage /CustomDataImage:C:\Recovery\Customizations\USMT.ppkg /ImagePath:C:\ /SingleInstance
		Warning:Do not put quotes with the /ImagePath:C:\ option.
//		<2>Cleanup the Windows files:
		mkdir temp
		Dism /Cleanup-Image /ImagePath:C:\ /StartComponentCleanup /ResetBase /ScratchDir:C:\Temp
//	Step 5:Capture the image
		Note:Capture the image of the Windows partition, and then copy it to the external drive or share
		Dism /Capture-Image /CaptureDir:C:\ /ImageFile:"C\WindowsWithOffice.wim" /Name:"Frech and desktop applicetions"
//	Step 6:Optional:Apply the image to a new device(factory process)
//		Prepare a place to store your files
//		<1>.For this step, you'll need another USB key, or an external hard drive or network location that can handle large files
//		<2>.Copy C:\WindowsWithOffice.wim to the storage USB drive or network share
		
//		Boot to Windows PE
//		<1>.Connect the WinPE USB drive to your reference device
//		<2>.Turn off the device,and then boot to the Windows PE USB drive.You usually do this by powering on the device and quickly pressing a key.
		WinPE starts at a command line, and runs wpeinit to set up the system
		
//		Format and set up the hard drive partitions on the reference device
//		<1>.On the reference device,connect the storage USB key or network share
//		<2>.Find the drive letters by using DiskPart:
		diskpart
		DISKPART> list volume
		DISKPART> exit
//		<3>.Format the primary hard drive,create the partitions, and apply the image by using the pre-made lab script
		D:
		Walkthrough-Deploy.bat "D:\WindowsWithOffice.wim"
//		<4>Shut down or reboot your device, either by holding down the power button for a full five seconds, or by using the following command:
		wpeutil shutdown


/********************************************************************************************************************/
//4.Windows updates
//	Step 1:Mount the Windows image file
	mkdir C:\mount\windows
	Dism /Mount-Image /ImageFile:"C:\Image\WindowsWithOffice.wim" /Index:1 /MountDir:"C:\mount\windows" /Optimize
//	Step 2:Add the Windows update package
//		<1.From Microsoft Connect,download the Windows update.Save this in the folder:C:\WindowsUpdates

//		<2.Add the updates to the image
		Dism /Add-Package /Image:"C\mount\windows" /PackagePath="C:\WindowsUpdates\kb1010101.cab" /PackagePath="C:\WindowsUpdates\kb1020202.cab" /PackagePath="C:\WindowsUpdates\kb1030303.cab" /LogPath=C:\mount\dism.log\

//		<3.Lock in the updates,so that they are restored during a recovery
		Dism /Cleanup-Image /Image=C:\ /StartComponentCleanup /ResetBase /ScratchDir:C:\Temp
//	Step 3:Unmount the images
//		<1.Close all applications that might access files from the image

//		<2.Commit the changes and unmount the Windows image:
		Dism /Unmount-Image /MountDir:"C:\mount\windows" /Commit


/********************************************************************************************************************/
//5.Keeping Windows settings through a recovery
