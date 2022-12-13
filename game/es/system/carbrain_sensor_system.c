#include "carbrain_sensor_system.h"

#include "../../carbrain/carbrain_io.h"
#include "../../carbrain/carbrain_es_integration.h"


//FIXME: finish this function
void system_carbrain_sensor_update(Engine* engine) {
    //only in part2! You can leave this empty in part1
    //TODO search all carbrains, check their surroundings, and send the info about them to the Sensor module on their "Board".
    printf("system_carbrain_sensor_update:");
    EntityIterator car_brains;
    search_entity_1(engine, COMP_CARBRAIN, &car_brains);
    while (next_entity(&car_brains)) {
        EntityId car_brain_id = car_brains.entity_id;
        CarBrainComponent* car_brain_comp = get_component(engine, car_brain_id, COMP_CARBRAIN);
        SensorReading sensor_reading = sense_environment(engine, car_brain_id);
        set_carbrain_sense(car_brain_comp, &sensor_reading);
    }
}

//TODO: implement sense_environment -> inspect every position around the carbrain, and return the info about them.
SensorReading sense_environment(Engine *engine, EntityId carbrain_entity_id) {
    SensorReading sensor_reading;
    LocationComponent* loc_comp = get_component(engine, carbrain_entity_id, COMP_LOCATION);
    DirectionComponent* dir_comp = get_component(engine, carbrain_entity_id, COMP_DIRECTION);
    t_vec3 car_brain_pos;
    glmc_assign_vec3(car_brain_pos, loc_comp -> pos);
    sensor_reading.crates_dragged = crates(engine, carbrain_entity_id);

    t_vec3 search_pos;
    
    for (size_t i = 0; i < 8; i++) {
        set_search_pos(direction8_to_direction(dir_comp->direction), search_pos, car_brain_pos, i);
        sensor_reading.bitmasks[i] = sense_pos(engine, search_pos);
    }
    
    return sensor_reading;
}

uint8_t sense_pos(Engine *engine, t_vec3 pos) {
    uint8_t res = 0;
    EntityIterator comp;

    search_entity_by_location_1(engine, pos, 0.3f, 0, COMP_ROAD, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_ROAD;
    }

    search_entity_by_location_1(engine, pos, 0.3f, 0, COMP_EXIT, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_EXIT;
    }

    search_entity_by_location_1(engine, pos, 0.3f, 0, COMP_MARKER, &comp);
    if (next_entity(&comp) == 1) {
        MarkerComponent* marker = get_component(engine, comp.entity_id, COMP_MARKER);
        if (marker -> color == LEVEL_COLOR_GREY) {
            res = res | SENSE_MARKER_A;
        } else if (marker -> color == LEVEL_COLOR_RAINBOW) {
            res = res | SENSE_MARKER_B;
        } else if (marker -> color == LEVEL_COLOR_0) {
            res = res | SENSE_MARKER_C;
        } else if (marker -> color == LEVEL_COLOR_1) {
            res = res | SENSE_MARKER_D;
        }
    }

    search_entity_by_location_1(engine, pos, 0.3f, 0, COMP_FILTER, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_FILTER;
    }

    search_entity_by_location_1(engine, pos, 0.3f, 0, COMP_DRAGGABLE, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_CRATE;
    }

    return res;
}


void set_search_pos(Direction carbrain_direction, t_vec3 pos, t_vec3 car_pos, size_t sensor_index) {
    switch (carbrain_direction)
    {
    case N:
        pos[0] = car_pos[0] + sensor_x_offset(sensor_index);
        pos[1] = car_pos[1] + sensor_y_offset(sensor_index);
        break;
    
    case W:
        pos[0] = car_pos[0] - sensor_y_offset(sensor_index);
        pos[1] = car_pos[1] + sensor_x_offset(sensor_index);
        break;

    case S:
        pos[0] = car_pos[0] - sensor_x_offset(sensor_index);
        pos[1] = car_pos[1] - sensor_y_offset(sensor_index);
        break;

    case E:
        pos[0] = car_pos[0] + sensor_y_offset(sensor_index);
        pos[1] = car_pos[1] - sensor_x_offset(sensor_index);
        break;

    default:
        break;
    }
    printf("x: %f, y: %f", pos[0], pos[1]);
}

int crates(Engine* engine, EntityId car){
    int i = 0;

    if(!has_component(engine,car,COMP_DRAGGING)){
        return i;
    }
    i++;
    DraggingComponent* dragging= get_component(engine,car,COMP_DRAGGING);
    DraggableComponent *crate = get_component(engine, dragging->first_crate,COMP_DRAGGABLE);
    while (crate->next_crate!=NO_ENTITY)
    {
        i++;
        crate=get_component(engine,crate->next_crate,COMP_DRAGGABLE);
    }
    return i;
}
