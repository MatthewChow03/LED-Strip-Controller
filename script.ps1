<#
#   Powershell terminal UI for the IR transceiver firmware
#   Includes outputing to and reading from the serial interface
#>

Clear-Host

$init_text = 'IR Transceiver Activated'

Write-Output "$init_text."

Write-Warning 'This is an awesome program.' 

# Get the available COM ports and assign the first port to the variable
$com_port = [System.IO.Ports.SerialPort]::getportnames()
$com_port = $com_port.Substring(0, 4)
Write-Output "Using port $com_port"

# Start the serial interface
$port = new-Object System.IO.Ports.SerialPort $com_port, 9600, None, 8, one
$port.open()

# Variables for the main loop
$port.ReadTimeout = 400
$loop_state = 1
$rx_data

[hashtable]$command_list = @{
    UP     = "Change Colour Up"; 
    DOWN   = "Change Colour Down";
    BRIGHT = "Brightness Up";
    DIM    = "Brightness Down";
    WHITE  = "Set Colour White";
    RED    = "Set Colour Red";
    BLUE   = "Set Colour Blue";
    GREEN  = "Set Colour Green";
    TOGGLE  = "Toggle On / Off";
}

# Main loop
while ($loop_state -ne 0) {
    # Command Mode
    if ($loop_state -eq 1) {
        # Prompt the user for commands
        $command = Read-Host 'Enter a command' 
        $port.WriteLine($command)
        Write-Output "`n"

        # Read Serial monitor and output values until timeout it reached
        try {
            # Clear extraneous messages that came after a previous timeout
            $port.DiscardInBuffer()

            while ($true) {
                $port.ReadLine()
            }
        }
        catch {
            #Write-Warning "Timeout"
        }
        
        # Prompt user to continue, exit, or perform other functions
        Write-Output "`n 0 | Exit Program`n 1 | Command Mode`n 2 | Command Help`n"
        [int]$loop_state = Read-Host 'Selection'
    }
    # Command Help
    elseif ($loop_state -eq 2) {
        # Display the command list
        Write-Output "Command  `tDescription"
        Write-Output "------- `t-----------`n"
        $command_list.GetEnumerator() | ForEach-Object {
        $message = "{0} `t `t{1}" -f $_.key, $_.value
        Write-Output $message
        }
        

        # Reset from previous assignment
        $Finished = "N"

        # Check if user can move back to the command mode
        while ($Finished -ine "Y") {
            Write-Output "`n`nReturn to Command Mode? (Y / N)" 
            $Finished = Read-Host "Selection"
        }
        $loop_state = 1
    }

    else {
        Write-Output "`n 0 | Exit Program`n 1 | Command Mode`n 2 | Command Help`n"
        [int]$loop_state = Read-Host 'Selection' 
    }

    Clear-Host
}

$port.Close()
