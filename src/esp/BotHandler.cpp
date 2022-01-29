#include "BotHandler.h"

BotHandler::BotHandler(WiFiClientSecure &client) : bot(this->BOT_TOKEN, client)
{
    // for (int i = 0; i < (sizeof(this->permittedChatIds) / sizeof(*this->permittedChatIds)); i++)
    //     this->permittedChatIds[i] = this->chat_ids[i];
}

BotHandler::BotHandler(const String &token, WiFiClientSecure &client, String chatIds[]) : bot(token, client)
{
    for (int i = 0; i < (sizeof(this->permittedChatIds) / sizeof(*this->permittedChatIds)); i++)
        this->permittedChatIds[i] = chatIds[i];
}

BotHandler::~BotHandler()
{
}

int BotHandler::getUpdates()
{
    return this->bot.getUpdates(this->bot.last_message_received + 1);
}

bool BotHandler::isIdPermitted(String id, bool shouldCheckPermission = true)
{
    bool isIdPermitted = false;

    if (shouldCheckPermission)
    {
        for (int i = 0; i < (sizeof(this->permittedChatIds) / sizeof(*this->permittedChatIds)); i++)
        {
            if (id == this->permittedChatIds[i])
                isIdPermitted = true;
        }
    }
    else
    {
        for (int i = 0; i < MAXCHATIDS; i++)
        {
            if (id == this->chatIds[i])
                isIdPermitted = true;
        }
    }

    return isIdPermitted;
}

void BotHandler::sendMessage(String chatId, String text)
{
    this->bot.sendMessage(chatId, text, "");
}

telegramMessage BotHandler::getMessage(int index)
{
    return this->bot.messages[index];
}

void BotHandler::begin(SerialCommunication *softwareSerial, State *state)
{
    this->mySerial = softwareSerial;
    this->mspState = state;

    this->botCommands = ("[" +
                         String("{\"command\":\"/start\", \"description\":\"Start bot\"},") +
                         String("{\"command\":\"/setautomatic\",\"description\":\"Set mode to automatic\"},") +
                         String("{\"command\":\"/setmanual\",\"description\":\"Set mode to manual\"},") +

                         String("{\"command\":\"/changetime\",\"description\":\"Manual - Change time\"},") +
                         String("{\"command\":\"/togglelight\",\"description\":\"Manual - Turn on and off light\"},") +
                         String("{\"command\":\"/togglewater\",\"description\":\"Manual - Open and close water\"},") +
                         String("{\"command\":\"/togglefan\",\"description\":\"Manual - Turn on and off fan\"},") +

                         String("{\"command\":\"/state\",\"description\":\"Request current state\"},") +
                         String("{\"command\":\"/help\",\"description\":\"Get help\"}") +
                         "]");
}

void BotHandler::setCommands()
{
    this->bot.setMyCommands(this->botCommands);
}

void BotHandler::setCommands(const String &commands)
{
    this->bot.setMyCommands(commands);
}

void BotHandler::startMessage(String chatId, String fromName)
{
    String welcome = "Welcome, " + fromName + ".\n";
    welcome += "Use the following commands to control your green house.\n\n";
    welcome += "/setautomatic to set mode to automatic\n";
    welcome += "/setmanual to set mode to manual\n";
    welcome += "/changetime to change time (only manual)\n";
    welcome += "/togglelight to turn on/off the light (only manual)\n";
    welcome += "/togglewater to turn on/off the water pump (only manual)\n";
    welcome += "/togglefan to turn on/off the fan (only manual)\n";
    welcome += "/state to request current mode state\n";
    welcome += "/help to get the command list\n";

    this->sendMessage(chatId, welcome);
}

void BotHandler::setAutomatic(String chatId)
{
    // this->mySerial->send("/cautomatic");
    this->sendMessage(chatId, "Mode set to automatic.");
}

void BotHandler::setManual(String chatId)
{
    // this->mySerial->send("/cmanual");
    this->sendMessage(chatId, "Mode set to manual.");
    this->sendMessage(chatId, "Set time (hh:mm format)");

    this->shouldSetTime = true;
}

void BotHandler::state(String chatId)
{
    this->mySerial->send("/cstate");
    this->mySerial->send(String("/i" + chatId));

    this->sendMessage(chatId, String("Gathering information... please wait."));
}

void BotHandler::help(String chatId)
{
    String help = "Command list.\n";
    help += "Use the following commands to control your green house.\n\n";
    help += "/setautomatic to set mode to automatic\n";
    help += "/setmanual to set mode to manual \n\n";
    help += "/changetime to change time (only manual)\n";
    help += "/togglelight to turn on/off the light (only manual)\n";
    help += "/togglewater to turn on/off the water pump (only manual)\n";
    help += "/togglefan to turn on/off the fan (only manual) \n\n";
    help += "/state to request current mode state\n";
    help += "/help to get the command list\n";

    this->sendMessage(chatId, help);
}

void BotHandler::handleNewMessages(int newMessages, bool isConnected)
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

        if (!isConnected)
        {
            this->sendMessage(chatId, "I'm sorry " + fromName + ",\n I cannot communicate with the greenhouse.");
            continue;
        }

        if (text == "/start")
        {
            if (!this->isIdPermitted(chatId))
            { // user not logged yet
                if (!this->isIdPermitted(chatId, false))
                { // user not loggin in yet
                    if (this->trying_chatIds_number < MAXCHATIDS)
                    {
                        this.chatIds[this->trying_chatIds_number++] = chatId;
                    }
                    else
                    {
                        this->sendMessage(chatId, "Too many users are trying to connect, sorry.");
                        continue;
                    }
                    // this->sendMessage(chatId, "To control the green house please send the password");
                }
                // else
                // { // user already loggin in
                this->sendMessage(chatId, "To control the green house please send the password");
                // }
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
                    if (this->permitted_chatIds_number < MAXCHATIDS)
                    {
                        this->permittedChatIds[this->permitted_chatIds_number++] = chatId;
                        this->startMessage(chatId, fromName);

                        // togliere chatId da chatIds
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

        // if (text == "/start")
        //     this->startMessage(chatId, fromName);
        if (text == "/setautomatic")
        {
            // this->setAutomatic(chatId);
            this->mySerial->send("/cautomatic");
            this->mySerial->send(String("/i" + chatId));
        }
        else if (text == "/setmanual")
        {
            // this->setManual(chatId);
            this->mySerial->send("/cmanual");
            this->mySerial->send(String("/i" + chatId));
        }
        else if (text == "/state")
        {
            this->state(chatId);
        }
        else if (text == "/help")
        {
            this->help(chatId);
        }
        else if (text == "/changetime")
        {
            if (*this->mspState == State_MANUAL)
            {
                this->sendMessage(chatId, "Set time (hh:mm format)");
            }
            else
            {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else if (text == "/togglelight")
        {
            if (*this->mspState == State_MANUAL)
            {
                this->mySerial->send("/ctogglelight");
                this->mySerial->send(String("/i" + chatId));
            }
            else
            {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else if (text == "/togglewater")
        {
            if (*this->mspState == State_MANUAL)
            {
                this->mySerial->send("/ctogglewater");
                this->mySerial->send(String("/i" + chatId));
            }
            else
            {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else if (text == "/togglefan")
        {
            if (*this->mspState == State_MANUAL)
            {
                this->mySerial->send("/ctogglefan");
                this->mySerial->send(String("/i" + chatId));
            }
            else
            {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else
        { // time
            if (this->shouldSetTime)
            {
                bool isCorrect = false;
                isCorrect = (text.length() <= 5) && isdigit(text.charAt(0)) && isdigit(text.charAt(1)) && (text.charAt(2) == ':') && isdigit(text.charAt(3)) && isdigit(text.charAt(4));

                if (isCorrect)
                {
                    if ((text.charAt(0) - '0') <= 1)
                    {
                        isCorrect = isdigit(text.charAt(1)) && (text.charAt(1) - '0') <= 9;
                    }
                    else if ((text.charAt(0) - '0') == 2)
                    {
                        isCorrect = isdigit(text.charAt(1)) && (text.charAt(1) - '0') <= 3;
                    }
                    else
                    {
                        isCorrect = false;
                    }

                    isCorrect = (isdigit(text.charAt(3)) && (text.charAt(3) - '0') <= 5) && (isdigit(text.charAt(4)) && (text.charAt(4) - '0') <= 9);
                }

                if (isCorrect)
                    this->mySerial->send(String("/d" + text));
                else
                    this->sendMessage(chatId, "Set time again, the correct format is hh:mm");
            }
            else
            {
                this->sendMessage(chatId, "Please send a command.");
            }
        }
    }
}

void BotHandler::start(bool isESPConnectedToMSP)
{
    if (millis() > this->lastTimeBotRan + this->botRequestDelay)
    {
        int numNewMessages = this->getUpdates();

        while (numNewMessages)
        {
#ifdef DEBUG
            Serial.println("got response");
#endif
            this->handleNewMessages(numNewMessages, isESPConnectedToMSP);
            numNewMessages = this->getUpdates();
        }

        this->lastTimeBotRan = millis();
    }
}