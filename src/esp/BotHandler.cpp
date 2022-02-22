#include "BotHandler.h"

BotHandler::BotHandler(FileSystem *fileSystem) : bot(this->BOT_TOKEN, this->client)
{
    this->fileSystem = fileSystem;
    this->client.setInsecure();
}

BotHandler::BotHandler(WiFiClientSecure &client) : bot(this->BOT_TOKEN, client)
{
}

BotHandler::BotHandler(const String &token, WiFiClientSecure &client, String chatId) : bot(token, client)
{
    this->logginChatId = chatId;
    this->permittedChatId = chatId;
}

BotHandler::~BotHandler()
{
}

// public 
void BotHandler::setCommands()
{
    this->bot.setMyCommands(this->botCommands);
}

void BotHandler::setCommands(const String &commands)
{
    this->bot.setMyCommands(commands);
}

void BotHandler::begin(String chatId, State *lightState, State *fanState, State *waterState, const String &ipAddress)
{
    if (chatId != "")
    {
        this->permittedChatId = chatId;
        this->logginChatId = chatId;

        this->sendMessage(chatId, "We are back online, thank you for waiting.");
    }

    this->lightState = lightState;
    this->fanState = fanState;
    this->waterState = waterState;

    this->ipAddress = ipAddress;

    this->botCommands = ("[" +
                        String("{\"command\":\"/start\", \"description\":\"Start bot\"},") +
                        String("{\"command\":\"/setautomatic\",\"description\":\"Set mode to automatic\"},") +
                        
                        String("{\"command\":\"/getipaddress\",\"description\":\"Get ip adress\"},") +
                        String("{\"command\":\"/togglelight\",\"description\":\"Turn on, off, and auto the light\"},") +
                        String("{\"command\":\"/togglewater\",\"description\":\"Open, close, and auto the water\"},") +
                        String("{\"command\":\"/togglefan\",\"description\":\"Turn on, off, and auto the fan\"},") +

                        String("{\"command\":\"/state\",\"description\":\"Request current state\"},") +
                        String("{\"command\":\"/end\",\"description\":\"Close connection\"},") +
                        String("{\"command\":\"/help\",\"description\":\"Get help\"}") +
                         "]");

    this->commandsList = (
                        String("Use the following commands to control your green house.\n\n") + 
                        String("/setautomatic to set mode to automatic\n") + 
                        String("/getipaddress to get the ip address\n") + 
                        String("/togglelight to turn on/off the light (on/off/auto)\n") + 
                        String("/togglewater to turn on/off the water pump (on/off/auto)\n") + 
                        String("/togglefan to turn on/off the fan (on/off/auto)\n") + 
                        String("/state to request current mode state\n") + 
                        String("/end to close the connection\n") + 
                        String("/help to get the command list\n")
                        );
}

void BotHandler::sendMessage(String chatId, String text)
{
    this->bot.sendMessage(chatId, text, "");
}

void BotHandler::setAutomatic(String chatId)
{
    this->sendMessage(chatId, "Mode set to automatic.");
}

void BotHandler::start(DayCycle dayCycle, float soilSensorPercentage, bool shouldWatering)
{
    if (millis() > this->lastTimeBotRan + this->botRequestDelay)
    {
        this->dayCycle = dayCycle;
        this->soilSensorPercentage = soilSensorPercentage;
        this->shouldWatering = shouldWatering;

        int numNewMessages = this->getUpdates();

        while (numNewMessages)
        {
#ifdef DEBUG
            Serial.println("got response");
#endif
            this->handleNewMessages(numNewMessages);
            numNewMessages = this->getUpdates();
        }

        this->lastTimeBotRan = millis();
    }
}

// private 
int BotHandler::getUpdates()
{
    return this->bot.getUpdates(this->bot.last_message_received + 1);
}

telegramMessage BotHandler::getMessage(int index)
{
    return this->bot.messages[index];
}

bool BotHandler::isIdPermitted(String id, bool shouldCheckPermission = true)
{
    bool isIdPermitted = false;

    if (shouldCheckPermission)
    {
        if (this->permittedChatId != "" && this->permittedChatId == id)
            isIdPermitted = true;
    }
    else
    {
        if (this->logginChatId != "" && this->logginChatId == id)
            isIdPermitted = true;
    }

    return isIdPermitted;
}

void BotHandler::handleNewMessages(int newMessages)
{
    for (int i = 0; i < newMessages; i++)
    {
        telegramMessage message = this->getMessage(i);

        String chatId = message.chat_id;
        String text = message.text;
        String fromName = message.from_name;

#ifdef DEBUG
        Serial.println(text);
#endif

        if (text == "/start")
        {
            if (!this->isIdPermitted(chatId))
            { // user not logged yet
                if (!this->isIdPermitted(chatId, false))
                { // user not loggin in yet
                    if (this->logginChatId == "")
                    {
                        this->logginChatId = chatId;
                    }
                    else
                    {
                        this->sendMessage(chatId, "Too many users are trying to connect, sorry.");
                        continue;
                    }
                }
                this->sendMessage(chatId, "To control the green house please send the password");
            }
            else
            { // user already logged
                this->sendMessage(chatId, "You are already connected.");
                this->startMessage(chatId, fromName);
            }

            continue;
        }

        if (!this->isIdPermitted(chatId))
        { // user not logged yet
            if (this->isIdPermitted(chatId, false))
            { // user is trying to connect
                if (text == this->BOT_CONNECTION_PSW)
                { // authorized user
                    if (this->permittedChatId == "")
                    {
                        this->permittedChatId = chatId;

                        const size_t capacity = JSON_OBJECT_SIZE(24) + 420;
                        DynamicJsonDocument json(capacity);
                        if(!this->fileSystem->read("/config.json", &json))
                        {
#ifdef DEBUG
                            Serial.println("Errore nella lettura della configurazione.");
#endif
                        }

                        json["telegram_chat_id"] = chatId;
                        delay(100);
                        this->fileSystem->write("/config.json", json);

                        this->startMessage(chatId, fromName);
                    }
                    else
                    {
                        this->sendMessage(chatId, "Too many users are already connected, sorry.");
                        continue;
                    }
                }
                else
                {
                    this->sendMessage(chatId, "Sorry, the password is wrong, try again.");
                }
            }
            else
            {
                this->sendMessage(chatId, "To try connect please /start the initial procedure.");
            }

            continue;
        }

        if (text == "/setautomatic")
        {
            *this->lightState = State_AUTO;
            *this->fanState = State_AUTO;
            *this->waterState = State_AUTO;
            this->sendMessage(chatId, "Everything has been set back to AUTO");
        }
        else if (text == "/state")
        {
            this->state(chatId);
        }
        else if (text == "/help")
        {
            this->help(chatId);
        }
        else if (text == "/end")
        {
            this->sendMessage(chatId, "I'm disconnecting you, it was a pleasure to meet you, bye " + fromName + ".");
            this->permittedChatId = "";
            this->logginChatId = "";
        }
        else if (text == "/togglelight")
        {
            *this->lightState = State(((*this->lightState) + 1) % 3);
            this->sendMessage(chatId, "The light is: " + this->stateToString[*this->lightState]);
        }
        else if (text == "/togglewater")
        {
            *this->waterState = State(((*this->waterState) + 1) % 3);
            this->sendMessage(chatId, "The water is: " + this->stateToString[*this->waterState]);
        }
        else if (text == "/togglefan")
        {
            *this->fanState = State(((*this->fanState) + 1) % 3);
            this->sendMessage(chatId, "The fan is: " + this->stateToString[*this->fanState]);
        }
        else if (text == "/getipaddress")
        {
            this->sendMessage(chatId, "My ip adress is: " + String(this->ipAddress));
        }
        else
        {
            this->sendMessage(chatId, "Please send a valid command.");
        }
    }
}

void BotHandler::startMessage(String chatId, String fromName)
{
    String welcome = "Welcome, " + fromName + ".\n";
    welcome += this->commandsList;

    this->sendMessage(chatId, welcome);
}

void BotHandler::state(String chatId)
{
#ifdef DEBUG
    Serial.println(this->dayCycleToString[this->dayCycle]);
    Serial.println(this->stateToString[*this->lightState]);
    Serial.println(this->stateToString[*this->fanState]);
    Serial.println(String(this->soilSensorPercentage));
    Serial.println(this->waterStateToString[*this->waterState]);
#endif

    // this->sendMessage(chatId, String("Gathering information... please wait."));
    String stateMsg = "";
    switch (*this->lightState)
    {
    case State_AUTO:
        stateMsg += "Is " + this->dayCycleToString[this->dayCycle] + ", so the light is " + String((this->dayCycle == DayCycle_DAY)? "ON" : "OFF") + '\n';
        break;
    default:
        stateMsg += "The light is " + this->stateToString[*this->lightState] + '\n';
        break;
    }

    switch (*this->fanState)
    {
    case State_AUTO:
        stateMsg += "the fan is " + String((this->dayCycle == DayCycle_DAY)? "ON" : "OFF") + '\n';
        break;
    default:
        stateMsg += "the fan is " + this->stateToString[*this->fanState] + '\n';
        break;
    }

    switch (*this->waterState)
    {
    case State_AUTO:
        stateMsg += "the soil sensor is measuring " + String(this->soilSensorPercentage) + "% of humidity, so the water is " + String((this->shouldWatering)? "OPEN" : "CLOSED") + '\n';
        break;
    default:
        stateMsg += "the water is " + this->waterStateToString[*this->waterState] + '\n';
        break;
    }

    this->sendMessage(chatId, stateMsg);
}

void BotHandler::help(String chatId)
{
    String help = "Command list.\n";
    help += this->commandsList;

    this->sendMessage(chatId, help);
}