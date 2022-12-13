#include "carbrain_sensor_system.h"

#include "../../carbrain/carbrain_io.h"
#include "../../carbrain/carbrain_es_integration.h"


void system_carbrain_sensor_update(Engine* engine) {
    //printf("system_carbrain_sensor_update:");
    EntityIterator car_brains;
    search_entity_1(engine, COMP_CARBRAIN, &car_brains);
    while (next_entity(&car_brains)) {
        EntityId car_brain_id = car_brains.entity_id;
        CarBrainComponent* car_brain_comp = get_component(engine, car_brain_id, COMP_CARBRAIN);
        SensorReading sensor_reading = sense_environment(engine, car_brain_id);
        set_carbrain_sense(car_brain_comp, &sensor_reading);
    }
}

SensorReading sense_environment(Engine *engine, EntityId carbrain_entity_id) {
    SensorReading sensor_reading;
    LocationComponent* loc_comp = get_component(engine, carbrain_entity_id, COMP_LOCATION);
    DirectionComponent* dir_comp = get_component(engine, carbrain_entity_id, COMP_DIRECTION);
    t_vec3 car_brain_pos;
    glmc_assign_vec3(car_brain_pos, loc_comp->pos);
    sensor_reading.crates_dragged = crates(engine, carbrain_entity_id);

    t_vec3 search_pos;
    
    // printf("CAR POSITION: x: %f, y: %f \n", car_brain_pos[0], car_brain_pos[1]);
    // printf("CAR DIRECTION: %d \n", dir_comp->direction);
    // printf("-------------------- \n");
    for (size_t i = 0; i < 8; i++) {
        set_search_pos(direction8_to_direction(dir_comp->direction), search_pos, car_brain_pos, i);
        //printf("POS %d => x: %f, y: %f \n", i, search_pos[0], search_pos[1]);
        sensor_reading.bitmasks[i] = sense_pos(engine, search_pos);
    }
    
    return sensor_reading;
}

//Hulpfunctie die de bitmask van een positie teruggeeft.
uint8_t sense_pos(Engine *engine, t_vec3 pos) {
    uint8_t res = 0;
    EntityIterator comp;

    search_entity_by_location_1(engine, pos, 0.3f, 1, COMP_ROAD, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_ROAD;
    }

    search_entity_by_location_1(engine, pos, 0.3f, 1, COMP_EXIT, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_EXIT;
    }

    search_entity_by_location_1(engine, pos, 0.3f, 1, COMP_MARKER, &comp);
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

    search_entity_by_location_1(engine, pos, 0.3f, 1, COMP_FILTER, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_FILTER;
    }

    search_entity_by_location_1(engine, pos, 0.3f, 1, COMP_DRAGGABLE, &comp);
    if (next_entity(&comp) == 1) {
        res = res | SENSE_CRATE;
    }

    return res;
}

//Hulpfunctie die de offset van een sensor teruggeeft op basis van de richting van de carbrain.
void set_search_pos(Direction carbrain_direction, t_vec3 pos, t_vec3 car_pos, size_t sensor_index) {
    switch (carbrain_direction)
    {
    case N:
        pos[0] = car_pos[0] + STEP_SIZE*sensor_x_offset(sensor_index);
        pos[1] = car_pos[1] + STEP_SIZE*sensor_y_offset(sensor_index);
        break;
    
    case W:
        pos[0] = car_pos[0] - STEP_SIZE*sensor_y_offset(sensor_index);
        pos[1] = car_pos[1] + STEP_SIZE*sensor_x_offset(sensor_index);
        break;

    case S:
        pos[0] = car_pos[0] - STEP_SIZE*sensor_x_offset(sensor_index);
        pos[1] = car_pos[1] - STEP_SIZE*sensor_y_offset(sensor_index);
        break;

    case E:
        pos[0] = car_pos[0] + STEP_SIZE*sensor_y_offset(sensor_index);
        pos[1] = car_pos[1] - STEP_SIZE*sensor_x_offset(sensor_index);
        break;

    default:
        printf("ERROR: Invalid carbrain direction \n");
        break;
    }
    //printf("x: %f, y: %f \n", pos[0], pos[1]);
}

//Hulpfunctie die het aantal crates die een carbrain vast heeft, teruggeeft.
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
