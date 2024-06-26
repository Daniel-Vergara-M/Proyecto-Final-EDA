#ifndef TASK_H
#define TASK_H

#include "./note.h"

/**
 * @brief Estructura interna de una fecha
 */
struct Date
{
    int day;
    int month;
    int year;
    Date() : day(1), month(1), year(2000) {}
    Date(int day, int month, int year) : day(day), month(month), year(year) {}
    friend ostream &operator<<(ostream &os, const Date &date)
    {
        os << date.day << "/" << date.month << "/" << date.year;
        return os;
    }

    bool operator<(const Date &date)
    {
        if (this->year < date.year)
            return true;
        if (this->month < date.month)
            return true;
        if (this->day < date.day)
            return true;
        if (this->year > date.year)
            return false;
        if (this->month > date.month)
            return false;

        return false;
    }

    bool operator<=(const Date &date)
    {
        return *this < date || *this == date;
    }
    bool operator>(const Date &date)
    {
        return !(*this <= date);
    }
    bool operator>=(const Date &date)
    {
        return !(*this < date);
    }

    bool operator==(const Date &date)
    {
        return this->day == date.day &&
               this->month == date.month &&
               this->year == date.year;
    }
};

class Task
{
private:
    int id;
    string title;
    string description;
    Date dueDate;
    StatusType status;
    PriorityType priority;
    std::set<User *> assignees;
    User *reporter;
    vector<User *> watchers;
    stack<Note> notes;

public:
    /**
     * @brief Constructor vacío de la clase Task
     */
    Task() : title(""), description(""), dueDate(Date()), status(OPEN), priority(LOW), assignees({}), reporter(nullptr)
    {
        id = rand() % 1000 + 1;
    }
    /**
     * @brief Constructor de la clase Task
     */
    Task(string title, string description, User *reporter, Date dueDate) : title(title), description(description), dueDate(dueDate), status(OPEN), priority(LOW), assignees({}), reporter(reporter)
    {
        id = rand() % 1000 + 1;
    }

    // Getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    Date getDueDate() const { return dueDate; }
    StatusType getStatus() const { return status; }
    PriorityType getPriority() const { return priority; }
    std::set<User *> getAssignees() const { return assignees; }
    User *getReporter() const { return reporter; }
    vector<User *> getWatchers() const { return watchers; }
    stack<Note> getNotes() const { return notes; }
    
    

    // Setters

    void setId(int id) { this->id = id; }
    void setTitle(string title) { this->title = title; }
    void setDescription(string description) { this->description = description; }
    void setDueDate(Date dueDate) { this->dueDate = dueDate; }
    void setStatus(StatusType status) { this->status = status; }
    void setPriority(PriorityType priority) { this->priority = priority; }
    void setAssignees(std::set<User *> assignees) { this->assignees = assignees; }
    void addAssignee(User *assignee) { assignees.insert(assignee); }
    void removeAssignee(User *assignee) { assignees.erase(remove(assignees.begin(), assignees.end(), assignee), assignees.end()); }
    void setReporter(User *reporter) { this->reporter = reporter; }

    /**
     * @brief Actualiza un usuario
     * @param oldUser Usuario viejo
     * @param newUser Usuario nuevo
     * @return void
     */
    void updateAssignee(User *oldUser, User *newUser)
    {
        removeAssignee(oldUser);
        addAssignee(newUser);
    }

    /**
     * @brief Actualiza una nota
     * @param oldNote Nota vieja
     * @param newNote Nota nueva
     * @return void
     */
    void updateNote(Note oldNote, Note newNote)
    {
        stack<Note> temp;
        while (!notes.empty())
        {
            if (notes.top() == oldNote)
            {
                temp.push(newNote);
            }
            else
            {
                temp.push(notes.top());
            }
            notes.pop();
        }
        while (!temp.empty())
        {
            notes.push(temp.top());
            temp.pop();
        }
    }

    

    // Métodos

    /**
     * @brief Método para añadir un usuario a la lista de observadores de la tarea
     * @param watcher Usuario a añadir
     * @return void
     */
    void addWatcher(User *watcher)
    {
        if (find(watchers.begin(), watchers.end(), watcher) == watchers.end())
        {
            watchers.push_back(watcher);
            return;
        }

        if (watcher == reporter)
        {
            cout << "El usuario ya es el reportador de la tarea" << endl;
            return;
        }

        if (find(assignees.begin(), assignees.end(), watcher) != assignees.end())
        {
            cout << "El usuario ya es asignado de la tarea" << endl;
            return;
        }
        cout << "El usuario ya es espectador de la tarea" << endl;
    }
    /**
     * @brief Método para eliminar un usuario de la lista de observadores de la tarea
     * @param watcher Usuario a eliminar
     * @return void
     */
    void removeWatcher(User *watcher)
    {
        if (watchers.empty())
        {
            cout << "No hay espectadores que eliminar" << endl;
            return;
        }

        if (find(watchers.begin(), watchers.end(), watcher) == watchers.end())
        {
            cout << "El usuario no es espectador de la tarea" << endl;
            return;
        }
        watchers.erase(remove(watchers.begin(), watchers.end(), watcher), watchers.end());
    }

    /**
     * @brief Método para añadir una nota a la tarea
     * @param author Usuario que añade la nota
     * @param title Título de la nota
     * @param content Contenido de la nota
     * @return void
     */
    void addNote(User *author, string title, string content)
    {
        if (find(watchers.begin(), watchers.end(), author) != watchers.end())
        {
            cout << "El usuario no puede añadir notas a la tarea" << endl;
            return;
        }

        if (title.empty() || content.empty())
        {
            cout << "El título y el contenido de la nota no pueden estar vacíos" << endl;
            return;
        }

        if (title.size() > 50 || content.size() > 500)
        {
            cout << "El título no puede tener más de 50 caracteres y el contenido no puede tener más de 500 caracteres" << endl;
            return;
        }

        notes.push(Note(author, title, content));
    }

    /**
     * @brief Método para eliminar la última nota añadida a la tarea
     * @return void
     * @pre La pila de notas no está vacía
     */
    void removeNote()
    {
        if (!notes.empty())
        {
            notes.pop();
            return;
        }
        cout << "No hay notas que eliminar" << endl;
    }

    

    /**
     * @brief Muestra las notas en la tarea.
     * @return void
     * @pre No hay notas en la tarea.
     */
    void showNotes()
    {
        if (notes.empty())
        {
            cout << "No hay notas que mostrar" << endl;
            return;
        }
        stack<Note> temp = notes;
        while (!temp.empty())
        {
            cout << temp.top().getTitle() << ": " << temp.top().getContent() << endl;
            temp.pop();
        }
    }

    /**
     * @brief Destructor de la tarea, por medio de la eliminación de las notas hasta que este vacio.
     */
    ~Task()
    {
        while (!notes.empty())
        {
            notes.pop();
        }
    }

    // Sobrecarga de operadores

    /**
     * @brief Sobrecarga del operador de inserción
     * @param os Stream de salida.
     * @param task Tarea a imprimir
     * @return ostream&
     */
    friend ostream &operator<<(ostream &os, const Task &task)
    {
        os << "Title: " << task.getTitle() << endl;
        os << "Description: " << task.getDescription() << endl;
        os << "Due Date: " << task.getDueDate().day << "/" << task.getDueDate().month << "/" << task.getDueDate().year << endl;
        os << "Status: " << statusToString(task.getStatus()) << endl;
        os << "Priority: " << priorityToString(task.getPriority()) << endl;
        os << "Reporter: " << task.getReporter() << endl;
        os << "Assignees: ";
        for (User *assignee : task.getAssignees())
        {
            os << assignee << " ";
        }
        os << "Watchers: ";
        for (User *watcher : task.getWatchers())
        {
            os << watcher << " ";
        }
        os << endl;
        os << "Notes: ";
        stack<Note> temp = task.getNotes();
        while (!temp.empty())
        {
            os << temp.top().getTitle() << ": " << temp.top().getContent() << endl;
            temp.pop();
        }
        os << endl;
        return os;
    }

    /**
     * @brief Sobrecarga del operador de comparación menor que
     * @param task Tarea a comparar
     * @return bool
     */
    bool operator<(const Task &task) const
    {
        return this->dueDate.year < task.getDueDate().year;
    }

    /**
     * @brief Sobrecarga del operador de comparación igual que
     * @param task Tarea a comparar
     * @return bool
     */
    bool operator==(const Task &task) const
    {
        return this->id == task.getId();
    }
};

#endif // TASK_H