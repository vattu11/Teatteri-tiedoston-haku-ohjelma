/* Teatteri tiedoston haku ohjelma
 *
 * Ohjelma lukee teatteritiedot tiedostosta ja tallentaa ne tietorakenteeseen map.
 * Kun tiedot tallennettu tietorakenteeseen voidaan sulkea tiedosto, eikä tätä
 * tarvita enään. Seuraavaksi ohjelma kysyy käyttäjältä komentoa, jonka jälkeen
 * ohjelma tekee käyttäjän komennon mukaisen toiminnon. Komentoja joita käyttäjä
 * voi antaa ovat seuraavat: {"plays", "theatres", "theatres_of_play",
 *  "plays_in_theatre", "players_in_play" ,"plays_in_town", "quit"}
 * Jos komento syötetään väärin tulostetaan virheilmoitus.
 * Käyttäjän syötettyä komento "quit" ohjelma lopettaa toiminnan EXIT_SUCCESS:illa.
 *
 * Ohjelman kirjoittaja
 * Nimi: Valtteri Sutelainen
 *
 * */
#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

using namespace std;

// Tiedostotts olevien kenttien määrä
const int NUMBER_OF_FIELDS = 5;

// Command prompt
const string PROMPT = "the> ";

// Error ja muut viestit
const string EMPTY_FIELD = "Error: empty field in line ";
const string FILE_ERROR = "Error: input file cannot be opened";
const string WRONG_PARAMETERS = "Error: wrong number of parameters";
const string THEATRE_NOT_FOUND = "Error: unknown theatre";
const string PLAY_NOT_FOUND = "Error: unknown play";
const string PLAYER_NOT_FOUND = "Error: unknown player";
const string TOWN_NOT_FOUND = "Error: unknown town";
const string COMMAND_NOT_FOUND = "Error: unknown command";
const string NOT_AVAILABLE = "No plays available";

// Split funktio
vector<string> split(const string &str, char delim)
{
    vector<string> result = {""};
    bool inside_quotation = false;
    for (char current_char : str)
    {
        if (current_char == '"')
        {
            inside_quotation = not inside_quotation;
        }
        else if (current_char == delim and not inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    return result;
}

// Tarkistaa onko merkkijono tyhjä vai pelkkiä välilyöntejä
bool isStringEmpty(const std::string &str)
{
    // Jos merkkijono on tyhjä
    if (str.empty())
    {
        return true;
    }
    // Tarkistetaan onko merkkijono pelkkiä välilyöntejä. Käydään siis koko merkkijono läpi.
    bool allSpaces = std::all_of(str.begin(), str.end(), [](unsigned char character)
                                 { return std::isspace(character); });
    return allSpaces;
}

// Funktio joka tulostaa näyttelijät ja teatterin, esitelmän ja teatterin nimen perusteella. Command prompt (players_in_play)
void playersInPlayTwoParameters(std::map<int, std::vector<std::string>> &theatres_data, std::string play, std::string theatre)
{

    // Luodaan vektori johon tallennetaan teatterit ja näyttelijät (nimet)
    std::vector<std::pair<std::string, std::string>> theatresAndPlayers;
    int numberOfTheatres = 0;

    for (auto &theatreData : theatres_data)
    {
        if (theatreData.second[1] == theatre)
        {
            numberOfTheatres++;
        }

        if ((split(theatreData.second[2], '/')[0] == play || split(theatreData.second[2], '/')[1] == play) && theatreData.second[1] == theatre)
        {
            theatresAndPlayers.push_back(std::make_pair(theatreData.second[1], theatreData.second[3]));
        }
    }

    if (numberOfTheatres == 0)
    {
        cout << THEATRE_NOT_FOUND << '\n';
    }
    else if (theatresAndPlayers.empty())
    {
        cout << PLAY_NOT_FOUND << '\n';
    }
    else
    {
        // Järjestetään vektori aakkos järjestykseen
        std::sort(theatresAndPlayers.begin(), theatresAndPlayers.end());

        for (const auto &item : theatresAndPlayers)
        {
            cout << item.first << " : " << item.second << '\n';
        }
    }
}

// Funktio joka tulostaa näyttelijät ja teatterin, esitelmän nimen perusteella. Command prompt (players_in_play)
void playersInPlay(std::map<int, std::vector<std::string>> &theatres_data, std::string play)
{

    // Luodaan vektori, johon tallennetaan näyttelijät ja teatterit,
    // Tämä voidaan lajitella jälkeen päin helposti aakkosjärjestykseen.
    std::vector<std::pair<std::string, std::string>> theatresAndPlayers;

    for (auto &theatreData : theatres_data)
    {
        if (split(theatreData.second[2], '/')[0] == play || split(theatreData.second[2], '/')[1] == play)
        {
            theatresAndPlayers.push_back(std::make_pair(theatreData.second[1], theatreData.second[3]));
        }
    }

    if (theatresAndPlayers.empty())
    {
        cout << PLAY_NOT_FOUND << '\n';
    }
    else
    {
        // Lajitellaa näyttelijät aakkosjärjestykseen
        std::sort(theatresAndPlayers.begin(), theatresAndPlayers.end());

        for (const auto &data : theatresAndPlayers)
        {
            cout << data.first << " : " << data.second << '\n';
        }
    }
}

// Funktio, joka tulostaa näytelmien nimet tietyssä teatterissa. Command prompt (plays_in_theatre)
void playsIntheatre(map<int, vector<string>> theatres_data, string theatre)
{

    std::set<string> uniqueTheatres;
    int numOfPlays = 0;

    // Käydään läpi teatterit ja lisätään ne settiin, jolloin saadaan pois duplkaatit
    for (auto &theatreData : theatres_data)
    {
        if (theatreData.second[1] == theatre)
        {
            uniqueTheatres.insert(theatreData.second[2]);
        }
    }

    for (auto &theatreName : uniqueTheatres)
    {
        cout << theatreName << endl;
        numOfPlays++;
    }
    if (numOfPlays == 0)
    {
        cout << THEATRE_NOT_FOUND << endl;
    }
}

// Funktio, joka tulostaa näytelmien nimet tietyssä kaupungissa. Command prompt (plays_in_town)
void playsInTown(const std::map<int, std::vector<std::string>> &theatres_data, std::string town)
{

    int numOfTowns = 0;
    int numOfplays = 0;

    // Luodaan custom comparator TheatrePlay:lle
    using TheatrePlay = std::pair<std::string, std::string>;

    // custom comparator TheatrePlay:lle
    auto theatrePlayComparator = [](const TheatrePlay &lhs, const TheatrePlay &rhs)
    {
        if (lhs.first != rhs.first)
        {
            return lhs.first < rhs.first;
        }
        return lhs.second < rhs.second;
    };

    // Käytetään custom comparatoria mapissa

    std::map<TheatrePlay, std::vector<std::pair<int, std::string>>, decltype(theatrePlayComparator)> plays(theatrePlayComparator);

    // Loopataan teatterit ja lisätään näytelmät map:iin
    for (const auto &entry : theatres_data)
    {
        const std::vector<std::string> &data = entry.second;
        if (data[0] == town)
        {
            numOfTowns++;
            TheatrePlay key = {data[1], data[2]};
            plays[key].push_back({entry.first, data[4]});
        }
    }
    // Loopataan näytelmät ja tulostetaan nämä
    for (auto &entry : plays)
    {

        // Järjestetään istumapaikan saatavuus laskevaan järjestykseen entry ID:n perusteella
        std::sort(entry.second.begin(), entry.second.end(), std::greater<>());

        // Otetaan huomioon vain viimeisin esitys
        if (entry.second[0].second != "none")
        {
            if (entry.first.second.find('/') != std::string::npos)
            {

                std::cout << entry.first.first << " : " << split(entry.first.second, '/')[0] << " --- " << split(entry.first.second, '/')[1] << " : " << entry.second[0].second << "\n";
                numOfplays++;
            }
            else
            {
                std::cout << entry.first.first << " : " << entry.first.second << " : " << entry.second[0].second << "\n";
                numOfplays++;
            }
        }
    }
    if (numOfTowns == 0)
    {
        cout << TOWN_NOT_FOUND << endl;
    }
    else if (numOfplays == 0)
    {
        cout << NOT_AVAILABLE << endl;
    }
}

// Funktio, joka tulostaa teatterit, joissa esitetään tiettyä näytelmää. Command prompt (theatres_of_play)
void theatresOfPlay(map<int, vector<string>> theatres_data, string play)
{
    int numberOfTheatres = 0;
    set<string> printedTheatres;

    for (auto &theatreData : theatres_data)
    {
        if (split(theatreData.second[2], '/')[0] == play || split(theatreData.second[2], '/')[1] == play)
        {
            if (printedTheatres.insert(theatreData.second[1]).second)
            {
                cout << theatreData.second[1] << '\n';
                numberOfTheatres++;
            }
        }
    }

    if (numberOfTheatres == 0)
    {
        cout << PLAY_NOT_FOUND << '\n';
    }
}

// Funktio, joka tulostaa näytelmien nimet. Command prompt (plays)
void printPlays(map<int, vector<string>> theatres_data)
{

    std::set<string> uniquePlays;
    for (auto &theatreData : theatres_data)
    {

        uniquePlays.insert(theatreData.second[2]);
    }
    for (auto &play : uniquePlays)
    {

        if (play.find('/') != std::string::npos)
        {
            cout << split(play, '/')[0] << " *** " << split(play, '/')[1] << '\n';
        }
        else
        {
            cout << play << '\n';
        }
    }
}

// Funktio, joka tulostaa teatterien nimet. Command prompt (theatres)
void printTheatres(map<int, vector<string>> theatres_data)
{
    std::set<string> uniqueTheatres;
    for (auto &theatreData : theatres_data)
    {
        uniqueTheatres.insert(theatreData.second[1]);
    }
    for (auto &theatre : uniqueTheatres)
    {
        cout << theatre << endl;
    }
}

// Tarkistaa onko merkkijono listassa
bool ifStringInList(std::string str, std::vector<std::string> list)
{

    return std::find(list.begin(), list.end(), str) != list.end();
}

// Main funktio
int main()
{

    // Luodaan kartta teattereille. Tähän lisätään tiedostosta luetut tiedot.
    map<int, vector<string>> theatres_data;
    vector<string> theatreInfo;
    // Lista komennoista
    std::vector<std::string> commands = {"plays", "theatres", "theatres_of_play", "plays_in_theatre", "players_in_play", "plays_in_town"};

    // Avataan tiedosto
    ifstream file;
    std::string fileName;
    cout << "Input file: ";
    std::getline(std::cin, fileName);

    file.open(fileName);

    // Jos tiedosto ei aukea, tulostetaan virheilmoitus ja lopetetaan ohjelma EXIT_FAILUREEN.
    if (!file.is_open())
    {
        std::cout << FILE_ERROR;
        return EXIT_FAILURE;
    }

    // Luetaan tiedostosta rivi kerrallaan
    std::string line;
    int lineNumber = 1;

    while (std::getline(file, line))
    {

        theatreInfo.clear();

        // Jos riviltä puuttuu tietoja, tulostetaan virheilmoitus ja lopetetaan ohjelma EXIT_FAILUREEN.
        // Loopataan rivin tiedot ja lisätään ne STL data-rakenteeseen.
        for (auto element : split(line, ';'))
        {

            if (isStringEmpty(element) || line.empty() || split(line, ';').size() != NUMBER_OF_FIELDS)
            {
                std::cout << EMPTY_FIELD << lineNumber << "\n";
                file.close();
                return EXIT_FAILURE;
            }
            theatreInfo.push_back(element); // Lisätään tiedot vektoriin
        }

        theatres_data[lineNumber] = theatreInfo; // Tallennetaan teatterin tiedot karttaan
        lineNumber++;
    }
    file.close();

    std::string command;

    while (command != "quit")
    {

        std::cout << PROMPT;
        std::getline(std::cin, command);

        if (command == "quit")
        {
            return EXIT_SUCCESS;
        }

        // Tarkistetaan onko komento olemassa
        if ((ifStringInList(split(command, ' ')[0], commands)) == false)
        {
            std::cout << COMMAND_NOT_FOUND << "\n";
        }

        // Tarkistetaan onko komennon parametrit oikein
        // Tarkistetaan myös, jos komennot ovat oikein niin mikä fuktio suoritetaan
        else if ((ifStringInList(split(command, ' ')[0], commands)) == true)
        {

            if (split(command, ' ')[0] == "theatres_of_play")
            {
                theatresOfPlay(theatres_data, split(command, ' ')[1]);
            }
            else if (split(command, ' ')[0] == "players_in_play")
            {

                if (split(command, ' ').size() == 2)
                {

                    playersInPlay(theatres_data, split(command, ' ')[1]);
                }
                if (split(command, ' ').size() == 3)
                {
                    playersInPlayTwoParameters(theatres_data, split(command, ' ')[1], split(command, ' ')[2]);
                }
            }
            else if (split(command, ' ')[0] == "plays_in_town")
            {
                playsInTown(theatres_data, split(command, ' ')[1]);
            }
            else if (split(command, ' ')[0] == "plays_in_theatre")
            {
                playsIntheatre(theatres_data, split(command, ' ')[1]);
            }

            else if (split(command, ' ').size() != 1)
            {
                std::cout << WRONG_PARAMETERS << "\n";
            }
            else
            {
                if (command == "plays")
                {
                    printPlays(theatres_data);
                }
                if (command == "theatres")
                {
                    printTheatres(theatres_data);
                }
            }
        }
    }
}