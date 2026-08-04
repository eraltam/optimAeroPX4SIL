init_lat = aircraftInitial.lat_deg;
init_long = aircraftInitial.lon_deg;
init_altitude = aircraftInitial.alt_m;
init_yaw_deg = aircraftInitial.yawAngle_rad;
if ispc
    app_path = '"C:\Program Files\FlightGear 2024.1\bin\fgfs.exe"';
elseif ismac
    app_path = '/Applications/FlightGear.app/Contents/MacOS/fgfs';
elseif isunix
    app_path = '/usr/bin/fgfs';
else
    error('Unsupported operating system');
end
% find closest feasible frame rate given the fixed step size (rounding up)
frameRate_Hz = floor(1 / stepSize_s);

flightGearHost = "127.0.0.1";
flightGearTelnetPort = 5400;
if exist('visualizationParams', 'var')
    if isfield(visualizationParams, 'flightGearHost')
        flightGearHost = visualizationParams.flightGearHost;
    end
    if isfield(visualizationParams, 'flightGearTelnetPort')
        flightGearTelnetPort = visualizationParams.flightGearTelnetPort;
    end
end

% Check to see if F16 FlightGear visualization exist
f16DirName = 'C:\Program Files\FlightGear 2024.1\data\Aircraft\f16';
options = sprintf('--fdm=null --native-fdm=socket,in,%d,%s,5502,udp --aircraft=c172p --fog-fastest --disable-clouds --disable-sound', frameRate_Hz, char(flightGearHost));
switch lower(vehicleParams.type)
    case "f-16"
        if isdir(f16DirName)
            options = sprintf('--fdm=null --native-fdm=socket,in,%d,%s,5502,udp --aircraft=f16-block-52 --fog-fastest --disable-clouds --disable-sound', frameRate_Hz, char(flightGearHost));
        end
    case "hexarotor"
        options = sprintf('--fdm=null --native-fdm=socket,in,%d,%s,5502,udp --aircraft=bigHexy  --aircraft-dir="visualization/bigHexy" --telnet=%d --fog-fastest --disable-clouds --disable-sound', frameRate_Hz, char(flightGearHost), flightGearTelnetPort);
    case {"c172p", "c172pjsbsim"}
        % No custom aircraft package needed -- c172p is one of FlightGear's own stock aircraft
        % (unlike bigHexy/f16-block-52 above), and the default `options` string built above this
        % switch already targets --aircraft=c172p, so this case is just an explicit acknowledgment
        % that "c172p"/"c172pJSBSim" are real, supported vehicleParams.type values rather than
        % falling through to the "Unknown vehicle" warning in `otherwise` -- see
        % PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md §2.1. c172pJSBSim shares the same real
        % airframe/visual model, see PLAN_JSBSIM_SFUNCTION_HYBRID_C172P.md.
    case "c130jsbsim"
        % Community C130 Hercules package (org.flightgear.fgaddon.stable_2024 hangar catalog),
        % downloaded by the user into FlightGear's separate aircraft-hangar download directory --
        % NOT under the base fgdata Aircraft/ folder, so (unlike c172p) this needs an explicit
        % --aircraft-dir, same pattern as the "hexarotor" case above rather than the bare
        % --aircraft= the f16/c172p cases use. --fdm=null (set in `options` above) means the
        % package's own bundled YASim FDM is never invoked here -- this is purely the visual/3D
        % model, driven externally by this SIL's real JSBSim-S-Function plant
        % (PLAN_JSBSIM_SFUNCTION_F22_C130.md), same as every other vehicle in this switch.
        % c130-set.xml (plain transport) chosen over the ac130 (gunship)/c130k/kc130 (tanker)
        % variants also present in this package -- those are cosmetically different loadouts of
        % the same airframe, not different flight models under --fdm=null.
        c130DirName = 'C:\Users\Edison Altamirano\FlightGear\Downloads\Aircraft\org.flightgear.fgaddon.stable_2024\Aircraft\C130';
        if isdir(c130DirName)
            options = sprintf('--fdm=null --native-fdm=socket,in,%d,%s,5502,udp --aircraft=c130 --aircraft-dir="%s" --fog-fastest --disable-clouds --disable-sound', frameRate_Hz, char(flightGearHost), c130DirName);
        else
            warning("C130 FlightGear package not found at %s -- falling back to default visualization aircraft", c130DirName)
        end
    case "f22jsbsim"
        % Community Lockheed-Martin FA-22A Raptor package (same hangar catalog as C130 above),
        % downloaded by the user. Two -set.xml variants ship in this package (f22-jsbsim-set.xml,
        % f22-yasim-set.xml) -- the name match with this SIL's own "f22JSBSim" vehicle is
        % coincidental, not functional: --fdm=null means neither package FDM is ever invoked
        % either way, this is visual-model-only, same as the c130jsbsim case above. f22-jsbsim
        % picked for the name match/documentation clarity, not for any behavioral reason.
        f22DirName = 'C:\Users\Edison Altamirano\FlightGear\Downloads\Aircraft\org.flightgear.fgaddon.stable_2024\Aircraft\Lockheed-Martin-FA-22A-Raptor';
        if isdir(f22DirName)
            options = sprintf('--fdm=null --native-fdm=socket,in,%d,%s,5502,udp --aircraft=f22-jsbsim --aircraft-dir="%s" --fog-fastest --disable-clouds --disable-sound', frameRate_Hz, char(flightGearHost), f22DirName);
        else
            warning("F-22 FlightGear package not found at %s -- falling back to default visualization aircraft", f22DirName)
        end
    otherwise
        warning("Unknown vehicle for display")
end
           
initial_conditions = sprintf('--in-air=%s  --lat=%f --lon=%f --altitude=%f --heading=%f --timeofday=noon', 'false', init_lat, init_long, init_altitude, rad2deg(init_yaw_deg));
scenery = sprintf('--enable-terrasync');
use_additional_options = false;
additional_options = '';
if use_additional_options
    % Examples of potentially useful options:
    additional_options = '--airport=KEDW --runway=10L --altitude=7224 --offset-distance=4.72 --offset-azimuth=0';
end
command = sprintf('%s %s %s %s %s &', app_path, options, initial_conditions, scenery, additional_options);
system(command);
