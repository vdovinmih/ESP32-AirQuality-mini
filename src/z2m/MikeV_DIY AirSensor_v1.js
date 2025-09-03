const m = require('zigbee-herdsman-converters/lib/modernExtend');


const tempReporting = {min: 0, max: 60, change: 100};
const humReporting = {min: 0, max: 600, change: 2000};
const tvocReporting = {min: 0, max: 60, change: 50};

const definition = {
    zigbeeModel: ['AirSensor_v1'],
    model: 'AirSensor_v1',
    vendor: 'Mike_DIY',
    description: 'AirSensor v1',
    extend: [
        m.deviceEndpoints(
        {
            "endpoints":
                {
                    "10":10,
                    "ppb":11,
                }
        }),
        m.temperature(
            {
                reporting: tempReporting,
            }
        ),
        m.humidity(
            {
                reporting: humReporting,
            }
        ),
        m.numeric({
            endpointNames: ['ppb'],
            name: 'TVOC',
            cluster: 'genAnalogInput',
            attribute: "presentValue",
            description: 'TVOC ppb',
            unit: "ppb",
            precision: 2,
            scale: 1,
            access: 'STATE_GET',
            reporting: tvocReporting,
        }),
    ],
    meta: {"multiEndpoint":true},
};

module.exports = definition;
