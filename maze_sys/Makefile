empty:=

#######################################################################################################
CFLAGS_RELEASE=-O3 -ggdb3
CFLAGS_DEBUG=-O0 -ggdb3 -Wall -Werror
CFLAGS_COMBINED_RELEASE=$(CFLAGS_RELEASE) $(CFLAGS)
CFLAGS_COMBINED_DEBUG=$(CFLAGS_DEBUG) $(CFLAGS)
SUFFIX_RELEASE=_REL
SUFFIX_DEBUG=_DBG

SUFFIX_SELECTED=
CFLAGS_SELECTED=

TEMPORARY=
Q=@
Q_ARCH=> /dev/null
#######################################################################################################
CFLAGS_STATIC_LIBRARY=		-c
CFLAGS_INCLUDE_DIRS=		-I.
CFLAGS_COMMON=				-std=gnu++11 $(CFLAGS_INCLUDE_DIRS) $(CFLAGS_SELECTED)

#######################################################################################################
DEFAULT_CLEARER=
CHOSEN_GPP=
CHOSEN_NAM=				
MAIN_BASE_DIR=
GPP_EV3=					arm-none-linux-gnueabi-g++
GPP_EV3_NAM=_ev3
GPP_RSP=					arm-linux-gnueabihf-c++
GPP_RSP_NAM=_rsp
GPP_NORMAL=					g++
GPP_NORMAL_NAM=_native
ARCHIVE=					ar rvs

#######################################################################################################
COMMON_INCLUDES=			
COMMON_DEFINITIONS=
MAIN_BASE_DIR=				./build/

#######################################################################################################
GOOGLE_TEST_INCLUDE_DIRS=	-I./libraries/googletest -I./libraries/googletest/include
GOOGLE_TEST_SOURCES=		./libraries/__linux__/google_test.cc
GOOGLE_TEST_LIBRARY=		$(MAIN_BASE_DIR)obj/google_test$(SUFFIX_SELECTED).o

GOOGLE_MOCK_INCLUDE_DIRS=	-I./libraries/googlemock -I./libraries/googlemock/include $(GOOGLE_TEST_INCLUDE_DIRS)
GOOGLE_MOCK_SOURCES=		./libraries/__linux__/google_mock.cc
GOOGLE_MOCK_LIBRARY=		$(MAIN_BASE_DIR)obj/google_mock$(SUFFIX_SELECTED).o

GLM_INCLUDE_DIRS=			-I./libraries/glm
GLM_SOURCES=				./libraries/__linux__/glm.cpp
GLM_LIBRARY=				$(MAIN_BASE_DIR)obj/glm$(SUFFIX_SELECTED).o

NETWORKING_DEFINES=			-DHAS_SOCKLEN_T
NETWORKING_INCLUDE_DIRS=	-I./libraries/networking -I./libraries/networking/include
NETWORKING_SOURCES=			./libraries/networking/all_linux.c
NETWORKING_LIBRARY=			$(MAIN_BASE_DIR)obj/networking$(SUFFIX_SELECTED).o

NETWORKINGPP_SOURCES=			./libraries/networking/enetpp.cxx
NETWORKINGPP_LIBRARY=			$(MAIN_BASE_DIR)obj/networkingpp$(SUFFIX_SELECTED).o

GoogleTest: $(GOOGLE_TEST_SOURCES)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(GOOGLE_TEST_INCLUDE_DIRS) $(GOOGLE_TEST_SOURCES) -o $(GOOGLE_TEST_LIBRARY)
	
GoogleMock: $(GOOGLE_MOCK_SOURCES)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(GOOGLE_MOCK_INCLUDE_DIRS) $(GOOGLE_MOCK_SOURCES) -o $(GOOGLE_MOCK_LIBRARY)

GLM: $(GLM_SOURCES)
	@mkdir -p $(MAIN_BASE_DIR)obj
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(GLM_INCLUDE_DIRS) $(GLM_SOURCES) -o $(GLM_LIBRARY)

#ugh, C
Net: $(NETWORKING_SOURCES)
	@mkdir -p $(MAIN_BASE_DIR)obj/networking
	$(Q)$(subst ++,cc,$(CHOSEN_GPP)) $(CFLAGS_INCLUDE_DIRS) $(CFLAGS_SELECTED) $(CFLAGS_STATIC_LIBRARY) $(NETWORKING_DEFINES) $(NETWORKING_INCLUDE_DIRS) $(NETWORKING_SOURCES) -o $(NETWORKING_LIBRARY)

NetPP: $(NETWORKINGPP_SOURCES)
	@mkdir -p $(MAIN_BASE_DIR)obj/networking
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(NETWORKING_INCLUDE_DIRS) $(NETWORKING_DEFINES) $(NETWORKINGPP_SOURCES) -o $(NETWORKINGPP_LIBRARY)

#######################################################################################################
PROJECT_NAME=				main_ev3_application

COMMON_INCLUDE_DIRS=		$(GLM_INCLUDE_DIRS) \
							-I./common \
							-L$(MAIN_BASE_DIR) \
							$(NETWORKING_INCLUDE_DIRS)
							
INCLUDE_DIRS_PROJECT=		-I./$(PROJECT_NAME) \
							-I./$(PROJECT_NAME)/lib \
							$(COMMON_INCLUDE_DIRS)

#######################################################################################################
LIBCOMMON_INCLUDE_DIRS=		$(INCLUDE_DIRS_PROJECT)
LIBCOMMON_SOURCES=			$(shell find ./common/ -name '*.c*')
LIBCOMMON_OBJ_DIR=			$(MAIN_BASE_DIR)libcommon/
LIBCOMMON_OBJECTS=			$(addprefix $(LIBCOMMON_OBJ_DIR),$(LIBCOMMON_SOURCES))
LIBCOMMON_LIBRARY_NAME=		common$(SUFFIX_SELECTED)
LIBCOMMON_LIBRARY=			$(MAIN_BASE_DIR)lib$(LIBCOMMON_LIBRARY_NAME).a
LIBCOMMON_DEFINES=			$(COMMON_DEFINITIONS)

C_LIBCOMMON: DEFAULT_CLEARER=$(LIBCOMMON_OBJ_DIR)
C_LIBCOMMON: COMMON_INCLUDES=$(LIBCOMMON_INCLUDE_DIRS)
C_LIBCOMMON: $(LIBCOMMON_OBJECTS)
	$(Q)$(ARCHIVE) $(LIBCOMMON_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(Q_ARCH)

#######################################################################################################
COMMON_LIBRARIES=			$(GLM_LIBRARY) \
							-lpthread \
							-ldl \
							-lrt \
							-l$(LIBCOMMON_LIBRARY_NAME)

#######################################################################################################
EV3M_NAME=					main_ev3_application

INCLUDE_DIRS_EV3M=			-I./$(EV3M_NAME) \
							-I./$(EV3M_NAME)/lib \
							$(COMMON_INCLUDE_DIRS)


LIBEV3M_INCLUDE_DIRS=		$(INCLUDE_DIRS_EV3M)
LIBEV3M_SOURCES=			$(shell find ./$(EV3M_NAME)/lib/ -name '*.c*')
LIBEV3M_OBJ_DIR=			$(MAIN_BASE_DIR)lib$(EV3M_NAME)/
LIBEV3M_OBJECTS=			$(addprefix $(LIBEV3M_OBJ_DIR),$(LIBEV3M_SOURCES))
LIBEV3M_NAME=				$(EV3M_NAME)$(SUFFIX_SELECTED)
LIBEV3M_LIBRARY=			$(MAIN_BASE_DIR)lib$(LIBEV3M_NAME).a
LIBEV3M_DEFINES=			$(COMMON_DEFINITIONS) \
							-DNO_LINUX_HEADERS

EV3M_LIBRARIES=				$(COMMON_LIBRARIES) \
							-l $(LIBEV3M_NAME) \
							$(NETWORKING_LIBRARY) \
							$(NETWORKINGPP_LIBRARY)
EV3M_INCLUDE_DIRS=			$(INCLUDE_DIRS_EV3M)
EV3M_SOURCES=				$(shell find ./$(EV3M_NAME)/ ! -path "./$(EV3M_NAME)/lib/*" -name '*.c*')
EV3M_OBJ_DIR=				$(MAIN_BASE_DIR)obj/$(EV3M_NAME)/
EV3M_OBJECTS=				$(addprefix $(EV3M_OBJ_DIR),$(EV3M_SOURCES))
EV3M_EXECUTABLE=			$(MAIN_BASE_DIR)$(LIBEV3M_NAME).bin
EV3M_DEFINES=				$(COMMON_DEFINITIONS) \
							-D$(EV3M_NAME) \
							-DNO_LINUX_HEADERS

C_LIBEV3M: DEFAULT_CLEARER=$(LIBEV3M_OBJ_DIR)
C_LIBEV3M: COMMON_INCLUDES=$(LIBEV3M_INCLUDE_DIRS)
C_LIBEV3M: $(LIBEV3M_OBJECTS)
	$(Q)$(ARCHIVE) $(LIBEV3M_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(Q_ARCH)

C_EV3M: DEFAULT_CLEARER=$(EV3M_OBJ_DIR)
C_EV3M: COMMON_INCLUDES=$(EV3M_INCLUDE_DIRS)
C_EV3M: $(EV3M_OBJECTS)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(EV3M_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(EV3M_LIBRARIES) -o $(EV3M_EXECUTABLE)

#######################################################################################################
UWB_NAME=					uwb_location

INCLUDE_DIRS_UWB=			-I./$(UWB_NAME) \
							-I./$(UWB_NAME)/lib \
							$(COMMON_INCLUDE_DIRS)

LIBUWB_INCLUDE_DIRS=		$(INCLUDE_DIRS_UWB)
LIBUWB_SOURCES=				$(shell find ./$(UWB_NAME)/lib/ -name '*.c*')
LIBUWB_OBJ_DIR=				$(MAIN_BASE_DIR)lib$(UWB_NAME)/
LIBUWB_OBJECTS=				$(addprefix $(LIBUWB_OBJ_DIR),$(LIBUWB_SOURCES))
LIBUWB_NAME=				$(UWB_NAME)$(SUFFIX_SELECTED)
LIBUWB_LIBRARY=				$(MAIN_BASE_DIR)lib$(LIBUWB_NAME).a
LIBUWB_DEFINES=				$(COMMON_DEFINITIONS)

UWB_LIBRARIES=				$(COMMON_LIBRARIES) \
							-l$(LIBUWB_NAME)
UWB_INCLUDE_DIRS=			$(INCLUDE_DIRS_UWB)
UWB_SOURCES=				$(shell find ./$(UWB_NAME)/ ! -path "./$(UWB_NAME)/lib/*" -name '*.c*')
UWB_OBJ_DIR=				$(MAIN_BASE_DIR)obj/$(UWB_NAME)/
UWB_OBJECTS=				$(addprefix $(UWB_OBJ_DIR),$(UWB_SOURCES))
UWB_EXECUTABLE=				$(MAIN_BASE_DIR)$(LIBUWB_NAME).bin
UWB_DEFINES=				$(COMMON_DEFINITIONS) \
							-DUWB

C_LIBUWB: DEFAULT_CLEARER=$(LIBUWB_OBJ_DIR)
C_LIBUWB: COMMON_INCLUDES=$(LIBUWB_INCLUDE_DIRS)
C_LIBUWB: $(LIBUWB_OBJECTS)
	$(Q)$(ARCHIVE) $(LIBUWB_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(Q_ARCH)

C_UWB: DEFAULT_CLEARER=$(UWB_OBJ_DIR)
C_UWB: COMMON_INCLUDES=$(UWB_INCLUDE_DIRS)
C_UWB: $(UWB_OBJECTS)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(UWB_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(UWB_LIBRARIES) -o $(UWB_EXECUTABLE)
					
#######################################################################################################
PIX_NAME=					pix_location

INCLUDE_DIRS_PIX=			-I./$(PIX_NAME) \
							-I./$(PIX_NAME)/lib \
							$(COMMON_INCLUDE_DIRS) \
							-I./libraries/pixy/$(SUFFIX_SELECTED)/

LIBPIX_INCLUDE_DIRS=		$(INCLUDE_DIRS_PIX)
LIBPIX_SOURCES=				$(shell find ./$(PIX_NAME)/lib/ -name '*.c*')
LIBPIX_OBJ_DIR=				$(MAIN_BASE_DIR)lib$(PIX_NAME)/
LIBPIX_OBJECTS=				$(addprefix $(LIBPIX_OBJ_DIR),$(LIBPIX_SOURCES))
LIBPIX_NAME=				$(PIX_NAME)$(SUFFIX_SELECTED)
LIBPIX_LIBRARY=				$(MAIN_BASE_DIR)lib$(LIBPIX_NAME).a
LIBPIX_DEFINES=				$(COMMON_DEFINITIONS)

PIX_LIBRARIES=				$(COMMON_LIBRARIES) \
							-l$(LIBPIX_NAME)
#							-lpixy
PIX_INCLUDE_DIRS=			$(INCLUDE_DIRS_PIX)
PIX_SOURCES=				$(shell find ./$(PIX_NAME)/ ! -path "./$(PIX_NAME)/lib/*" -name '*.c*')
PIX_OBJ_DIR=				$(MAIN_BASE_DIR)obj/$(PIX_NAME)/
PIX_OBJECTS=				$(addprefix $(PIX_OBJ_DIR),$(PIX_SOURCES))
PIX_EXECUTABLE=				$(MAIN_BASE_DIR)$(LIBPIX_NAME).bin
PIX_DEFINES=				$(COMMON_DEFINITIONS) \
							-DPIX

C_LIBPIX: DEFAULT_CLEARER=$(LIBPIX_OBJ_DIR)
C_LIBPIX: COMMON_INCLUDES=$(LIBPIX_INCLUDE_DIRS)
C_LIBPIX: $(LIBPIX_OBJECTS)
	$(Q)$(ARCHIVE) $(LIBPIX_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(Q_ARCH)

C_PIX: DEFAULT_CLEARER=$(PIX_OBJ_DIR)
C_PIX: COMMON_INCLUDES=$(PIX_INCLUDE_DIRS)
C_PIX: $(PIX_OBJECTS)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(PIX_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(PIX_LIBRARIES) -o $(PIX_EXECUTABLE)

#######################################################################################################
SRVR_NAME=					main_server_application

INCLUDE_DIRS_SRVR=			-I./$(SRVR_NAME) \
							-I./$(SRVR_NAME)/lib \
							-I./$(PPR_NAME)/lib \
							$(COMMON_INCLUDE_DIRS)

LIBSRVR_INCLUDE_DIRS=		$(INCLUDE_DIRS_SRVR)
LIBSRVR_SOURCES=			$(shell find ./$(SRVR_NAME)/lib/ -name '*.c*')
LIBSRVR_OBJ_DIR=			$(MAIN_BASE_DIR)lib$(SRVR_NAME)/
LIBSRVR_OBJECTS=			$(addprefix $(LIBSRVR_OBJ_DIR),$(LIBSRVR_SOURCES))
LIBSRVR_NAME=				$(SRVR_NAME)$(SUFFIX_SELECTED)
LIBSRVR_LIBRARY=			$(MAIN_BASE_DIR)lib$(LIBSRVR_NAME).a
LIBSRVR_DEFINES=			$(COMMON_DEFINITIONS)

SRVR_LIBRARIES=				$(COMMON_LIBRARIES) \
							-l$(LIBSRVR_NAME) \
							-l$(LIBPPR_NAME) \
							$(NETWORKING_LIBRARY) \
							$(NETWORKINGPP_LIBRARY)
							
SRVR_INCLUDE_DIRS=			$(INCLUDE_DIRS_SRVR)
SRVR_SOURCES=				$(shell find ./$(SRVR_NAME)/ ! -path "./$(SRVR_NAME)/lib/*" -name '*.c*')
SRVR_OBJ_DIR=				$(MAIN_BASE_DIR)obj/$(SRVR_NAME)/
SRVR_OBJECTS=				$(addprefix $(SRVR_OBJ_DIR),$(SRVR_SOURCES))
SRVR_EXECUTABLE=			$(MAIN_BASE_DIR)$(LIBSRVR_NAME).bin
SRVR_DEFINES=				$(COMMON_DEFINITIONS) \
							-D$(SRVR_NAME) \
							$(NETWORKING_DEFINES)

C_LIBSRVR: DEFAULT_CLEARER=$(LIBSRVR_OBJ_DIR)
C_LIBSRVR: COMMON_INCLUDES=$(LIBSRVR_INCLUDE_DIRS)
C_LIBSRVR: $(LIBSRVR_OBJECTS)
	$(Q)$(ARCHIVE) $(LIBSRVR_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(Q_ARCH)

C_SRVR: DEFAULT_CLEARER=$(SRVR_OBJ_DIR)
C_SRVR: COMMON_INCLUDES=$(SRVR_INCLUDE_DIRS)
C_SRVR: $(SRVR_OBJECTS)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(SRVR_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(SRVR_LIBRARIES) -o $(SRVR_EXECUTABLE)


#######################################################################################################
PPR_NAME=					path_processor

INCLUDE_DIRS_PPR=			-I./$(PPR_NAME) \
							-I./$(PPR_NAME)/lib \
							$(COMMON_INCLUDE_DIRS)

LIBPPR_INCLUDE_DIRS=		$(INCLUDE_DIRS_PPR)
LIBPPR_SOURCES=				$(shell find ./$(PPR_NAME)/lib/ -name '*.c*')
LIBPPR_OBJ_DIR=				$(MAIN_BASE_DIR)lib$(PPR_NAME)/
LIBPPR_OBJECTS=				$(addprefix $(LIBPPR_OBJ_DIR),$(LIBPPR_SOURCES))
LIBPPR_NAME=				$(PPR_NAME)$(SUFFIX_SELECTED)
LIBPPR_LIBRARY=				$(MAIN_BASE_DIR)lib$(LIBPPR_NAME).a
LIBPPR_DEFINES=				$(COMMON_DEFINITIONS)

PPR_LIBRARIES=				-l$(LIBPPR_NAME) \
							$(COMMON_LIBRARIES) 
PPR_INCLUDE_DIRS=			$(INCLUDE_DIRS_PPR)
PPR_SOURCES=				$(shell find ./$(PPR_NAME)/ ! -path "./$(PPR_NAME)/lib/*" -name '*.c*')
PPR_OBJ_DIR=				$(MAIN_BASE_DIR)obj/$(PPR_NAME)/
PPR_OBJECTS=				$(addprefix $(PPR_OBJ_DIR),$(PPR_SOURCES))
PPR_EXECUTABLE=				$(MAIN_BASE_DIR)$(LIBPPR_NAME).bin
PPR_DEFINES=				$(COMMON_DEFINITIONS) \
							-D$(PPR_NAME)

C_LIBPPR: DEFAULT_CLEARER=$(LIBPPR_OBJ_DIR)
C_LIBPPR: COMMON_INCLUDES=$(LIBPPR_INCLUDE_DIRS)
C_LIBPPR: $(LIBPPR_OBJECTS)
	$(Q)$(ARCHIVE) $(LIBPPR_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(Q_ARCH)

C_PPR: DEFAULT_CLEARER=$(PPR_OBJ_DIR)
C_PPR: COMMON_INCLUDES=$(PPR_INCLUDE_DIRS)
C_PPR: $(PPR_OBJECTS)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(PPR_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(PPR_LIBRARIES) -o $(PPR_EXECUTABLE)

#######################################################################################################
TEST_INCLUDE_DIRS=			$(INCLUDE_DIRS_PROJECT) \
							$(INCLUDE_DIRS_UWB) \
							$(INCLUDE_DIRS_PIX) \
							$(GOOGLE_MOCK_INCLUDE_DIRS) \
							$(INCLUDE_DIRS_PPR) \
							$(INCLUDE_DIRS_SVR) 
TEST_SOURCES=				$(shell find ./test/ -name '*.c*')
TEST_OBJ_DIR=				$(MAIN_BASE_DIR)obj/test/
TEST_OBJECTS=				$(addprefix $(TEST_OBJ_DIR), $(TEST_SOURCES))
TEST_EXECUTABLE=			$(MAIN_BASE_DIR)test$(SUFFIX_SELECTED).bin
TEST_DEFINES=				$(COMMON_DEFINITIONS) \
							-DUNIT_TESTING_FRAMEWORK
TEST_LIBRARIES=				$(COMMON_LIBRARIES) \
							-l$(LIBEV3M_NAME) \
							-l$(LIBUWB_NAME) \
							-l$(LIBPPR_NAME) \
							-l$(LIBSRVR_NAME) \
							-l$(LIBPIX_NAME) \
							$(NETWORKING_LIBRARY) \
							$(NETWORKINGPP_LIBRARY) \
							$(GOOGLE_TEST_LIBRARY) \
							$(GOOGLE_MOCK_LIBRARY)

C_TEST: DEFAULT_CLEARER=$(TEST_OBJ_DIR)
C_TEST: COMMON_INCLUDES=$(TEST_INCLUDE_DIRS)
C_TEST: $(TEST_OBJECTS)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(TEST_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(TEST_LIBRARIES) -o $(TEST_EXECUTABLE)

#######################################################################################################
all: ev3_r ev3_tr rsp_r rsp_tr r tr

.DEFAULT:
	@mkdir -p ./$(@D)
	$(Q)$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(CFLAGS_INCLUDE_DIRS) $(COMMON_INCLUDES) ./$(subst $(DEFAULT_CLEARER),$(empty),./$@) -o ./$@$(SUFFIX_SELECTED).o

clean:
	@rm -rf $(MAIN_BASE_DIR)*

ev3_d ev3_td ev3_r ev3_tr: CHOSEN_GPP=$(GPP_EV3)
ev3_d ev3_td ev3_r ev3_tr: CHOSEN_NAM=$(GPP_EV3_NAM)
					
rsp_d rsp_td rsp_r rsp_tr: CHOSEN_GPP=$(GPP_RSP)
rsp_d rsp_td rsp_r rsp_tr: CHOSEN_NAM=$(GPP_RSP_NAM)

d td r tr: CHOSEN_GPP=$(GPP_NORMAL)
d td r tr: CHOSEN_NAM=$(GPP_NORMAL_NAM)

ev3_d ev3_td rsp_d rsp_td d td: SUFFIX_SELECTED=$(SUFFIX_DEBUG)$(CHOSEN_NAM)
ev3_d ev3_td rsp_d rsp_td d td: CFLAGS_SELECTED=$(CFLAGS_COMBINED_DEBUG)

ev3_r ev3_tr rsp_r rsp_tr r tr: SUFFIX_SELECTED=$(SUFFIX_RELEASE)$(CHOSEN_NAM)
ev3_r ev3_tr rsp_r rsp_tr r tr: CFLAGS_SELECTED=$(CFLAGS_COMBINED_RELEASE)

ev3_d ev3_r rsp_d rsp_r d r: Net NetPP GLM C_LIBCOMMON C_LIBEV3M C_LIBUWB C_LIBPIX C_LIBSRVR C_LIBPPR C_UWB C_PIX C_SRVR C_PPR C_EV3M

ev3_td ev3_tr rsp_tr rsp_td td tr: GoogleTest GoogleMock Net NetPP GLM C_LIBCOMMON C_LIBEV3M C_LIBUWB C_LIBPIX C_LIBSRVR C_LIBPPR C_TEST
#	valgrind $(TEST_EXECUTABLE)
