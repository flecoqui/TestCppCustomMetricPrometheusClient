# This script install :
# - tsroute  on a computer running Windows
#
#usage install-tsroute.ps1 dnsname

param
(
      [string]$dnsName = $null
)

set-executionpolicy remotesigned -Force

#Create Source folder
$source = 'C:\tsroute' 
If (!(Test-Path -Path $source -PathType Container)) {New-Item -Path $source -ItemType Directory | Out-Null} 
function WriteLog($msg)
{
Write-Host $msg
$msg >> c:\tsroute\test.log
}

if(!$dnsName) {
 WriteLog "DNSName not specified" 
 throw "DNSName not specified"
}

 WriteLog "Downloading tsroute"  
 $url = 'https://github.com/flecoqui/Win32/blob/master/TSRoute/Releases/ReleasesWithTSFiles.zip' 
 $webClient = New-Object System.Net.WebClient  
 $webClient.DownloadFile($url,$source + "\ReleasesWithTSFiles.zip" )  
 
 
 WriteLog "Installing tsroute"  
 # Function to unzip file contents 
 function Expand-ZIPFile($file, $destination) 
 { 
     $shell = new-object -com shell.application 
     $zip = $shell.NameSpace($file) 
     foreach($item in $zip.items()) 
     { 
         # Unzip the file with 0x14 (overwrite silently) 
         $shell.Namespace($destination).copyhere($item, 0x14) 
     } 
 } 
Expand-ZIPFile -file "$source\ReleasesWithTSFiles.zip" -destination $source 
WriteLog "TSRoute Installed"  



WriteLog "Configuring firewall" 
function Add-FirewallRules
{
netsh advfirewall firewall add rule name="rdp" dir=in action=allow protocol=TCP localport=3389
}
Add-FirewallRules
WriteLog "Firewall configured" 


WriteLog "Creating xml config file" 
$content = @'
<TSROUTE.InputParameters>
  <!-- Global parameters -->
  <TraceFile>TSROUTE.log</TraceFile>
  <TraceMaxSize>300000</TraceMaxSize>
  <TraceLevel>information</TraceLevel>
  <ConsoleTraceLevel>information</ConsoleTraceLevel>
  <!-- Stream 1 parameters -->
  <!-- MPEG4 SPTS -->
  <Stream>
    <Name>Stream1</Name>
    <OutputFile>Stream1.xml</OutputFile>
    <RefreshPeriod>5</RefreshPeriod>
    <TSFile>C:\tsroute\ReleasesWithTSFiles\TEST1.TS</TSFile>
    <UdpIpAddress>239.1.1.1</UdpIpAddress>
    <UdpPort>2501</UdpPort>
    <TTL>2</TTL>
    <Loop>-1</Loop>
    <UpdateTimeStamp>1</UpdateTimeStamp>
  </Stream>
</TSROUTE.InputParameters>
'@
$content | Out-File -FilePath C:\tsroute\ReleasesWithTSFiles\tsstreams.xml -Encoding utf8
WriteLog "Creating TSRoute Config file done" 

WriteLog "Installing TSRoute as a service" 
C:\tsroute\ReleasesWithTSFiles\tsroute.exe -install -xmlfile C:\tsroute\ReleasesWithTSFiles\tsstream.xml 
WriteLog "TSRoute Installed" 

WriteLog "Initialization completed !" 
WriteLog "Rebooting !" 
Restart-Computer -Force       
