#include <KWindowSystem.h>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Get a list of all currently open windows
    QList<WId> windowList = KWindowSystem::windows();

    // Iterate through the list and close each window
    foreach (WId window, windowList) {
        // Check if the window is a normal window (not a desktop or a panel)
        if (KWindowSystem::isNormalWindow(window)) {
            // Close the window
            KWindowSystem::closeWindow(window);
        }
    }

    // Return 0 to indicate successful execution
    return 0;
}
