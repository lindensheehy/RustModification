if (command == "project") {

    logNewLine();
    logWrite("Printing character list Entries with Projection...", true);

    if (!characters) {
        logWrite("Character list is null.", true);
        return;
    }

    if (characters->camera == nullptr) {
        logWrite("Need to assign player before trying to project data!", true);
        std::cout << "Need to assign player before trying to project data!\n";
        return;
    }

    // Loop over each character in the list
    for (int i = 0; i < characters->size; i++) {

        Character* character = characters->getCharacter(i);

        if (character == nullptr) break;

        // Log index label
        logNewLine();
        logWrite(std::to_string(i + 1) + ":", true);

        // Log world position
        logWrite("  World Position -> ", false);
        character->position->log();

        // Apply world to camera space transformation
        Vec3* cameraSpacePos = worldToCameraSpace(character->position, characters->camera->position, characters->camera->facing);
        if (cameraSpacePos == nullptr) {
            logWrite("cameraSpacePos is null!", true);
            std::cout << "cameraSpacePos is null!\n";
            continue;
        }

        // Log camera space position
        logWrite("  Camera Space Position -> ", false);
        cameraSpacePos->log();

        // Apply the new project function to convert to screen space (with depth)
        Vec3* projectedPos = project(cameraSpacePos); // This will handle all projection and conversion to screen space
        if (projectedPos == nullptr) {
            logWrite("projectedPos is null!", true);
            std::cout << "projectedPos is null!\n";
            continue;
        }

        // Log the projected position
        logWrite("  Screen Space Position -> ", false);
        projectedPos->log();

        // Log the depth (distance from the camera)
        logWrite("  Depth (Distance from Camera) -> ", false);
        std::cout << "  Depth: " << projectedPos->z << std::endl;

        // Clean up dynamically allocated memory for projection positions
        delete cameraSpacePos;
        delete projectedPos;  // No need to handle the other positions since `project` function combines them

    }

    logNewLine();
    std::cout << "Logged character list entries with projection!\n";

    return;
}