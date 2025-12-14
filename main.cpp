#include "/workspaces/C++/balagur_project_filled/include/NPC.h"
#include "/workspaces/C++/balagur_project_filled/include/NPCFactory.h"
#include <iostream>
#include <fstream>
#include <random>
#include <limits>

void save(const set_t &npcs, const std::string &filename) {
    std::ofstream fs(filename);
    if (!fs) return;
    fs << npcs.size() << std::endl;
    for (auto &npc : npcs) npc->save(fs), fs << std::endl;
    std::cout << "Сохранено " << npcs.size() << " NPC" << std::endl;
}

set_t load(const std::string &filename, NPCFactory &factory) {
    set_t result;
    std::ifstream is(filename);
    if (is) {
        int count; is >> count;
        for (int i = 0; i < count; ++i) {
            auto npc = factory.load(is);
            if (npc) result.insert(npc);
        }
        std::cout << "Загружено " << result.size() << " NPC" << std::endl;
    }
    return result;
}

void print_npcs(const set_t &npcs) {
    std::cout << "\nСписок NPC (" << npcs.size() << "):" << std::endl;
    for (auto &npc : npcs) npc->print();
}

set_t fight(const set_t &npcs, size_t distance) {
    set_t dead_list;
    for (const auto &attacker : npcs) {
        if (dead_list.find(attacker) != dead_list.end()) continue;
        for (const auto &defender : npcs) {
            if (attacker != defender && 
                attacker->is_close(defender, distance) &&
                dead_list.find(defender) == dead_list.end()) {
                if (defender->accept(attacker)) dead_list.insert(defender);
            }
        }
    }
    return dead_list;
}

void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    set_t npcs;
    NPCFactory factory;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int choice;
    do {
        std::cout << "\n=== ЭДИТОР ===\n";
        std::cout << "1. Добавить NPC\n";
        std::cout << "2. Показать NPC\n";
        std::cout << "3. Сохранить\n";
        std::cout << "4. Загрузить\n";
        std::cout << "5. Бой\n";
        std::cout << "6. Сгенерировать случайные NPC\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите: ";
        std::cin >> choice;
        clear_input();
        
        switch(choice) {
            case 1: {
                std::cout << "Тип (1-Рыцарь, 2-Орк, 3-Медведь): ";
                int type, x, y;
                std::string name;
                std::cin >> type >> x >> y >> name;
                if (auto npc = factory.create(static_cast<NPC::Type>(type), x, y, name))
                    npcs.insert(npc);
                else
                    std::cout << "Ошибка создания NPC!" << std::endl;
                break;
            }
            case 2: 
                print_npcs(npcs); 
                break;
            case 3: 
                save(npcs, "dungeon.txt"); 
                break;
            case 4: 
                npcs = load("dungeon.txt", factory); 
                break;
            case 5: {
                std::cout << "Дальность боя: ";
                size_t d; 
                std::cin >> d;
                auto dead = fight(npcs, d);
                for (auto &d : dead) npcs.erase(d);
                std::cout << "Убито: " << dead.size() 
                          << ", Осталось: " << npcs.size() << std::endl;
                break;
            }
            case 6: {
                std::cout << "Количество NPC для генерации: ";
                int n; 
                std::cin >> n;
                std::uniform_int_distribution<> coord(0, 500), type(1, 3);
                for (int i = 0; i < n; ++i) {
                    if (auto npc = factory.create(
                        static_cast<NPC::Type>(type(gen)), 
                        coord(gen), 
                        coord(gen)))
                        npcs.insert(npc);
                }
                std::cout << "Сгенерировано " << n << " NPC" << std::endl;
                break;
            }
            case 0:
                std::cout << "Выход из программы..." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор!" << std::endl;
                break;
        }
    } while (choice != 0);
    
    return 0;
}