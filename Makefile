.PHONY: clean All

All:
	@echo "----------Building project:[ spitest - Debug ]----------"
	@cd "spitest" && "$(MAKE)" -f  "spitest.mk"
clean:
	@echo "----------Cleaning project:[ spitest - Debug ]----------"
	@cd "spitest" && "$(MAKE)" -f  "spitest.mk" clean
